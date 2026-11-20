#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

namespace
{
	// Devuelve el id de red del jugador local, o nulo si Steam/OSS no lo tiene todavia
	FUniqueNetIdPtr ObtenerIdJugador(UWorld* World)
	{
		if (!World) return nullptr;
		const ULocalPlayer* JugadorLocal = World->GetFirstLocalPlayerFromController();
		if (!JugadorLocal) return nullptr;
		const FUniqueNetIdRepl Id = JugadorLocal->GetPreferredUniqueNetId();
		return Id.IsValid() ? Id.GetUniqueNetId() : nullptr;
	}

	FString NombreSubsistema()
	{
		IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
		return OSS ? OSS->GetSubsystemName().ToString() : TEXT("NINGUNO");
	}
}

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() :
	CrearSesionCompletadaDelegado(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::AlCrearSesionCompletada)),
	BuscarSesionesCompletadoDelegado(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::AlBuscarSesionesCompletado)),
	UnirseASesionCompletadoDelegado(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::AlUnirseASesionCompletado)),
	DestruirSesionCompletadaDelegado(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::AlDestruirSesionCompletada)),
	IniciarSesionCompletadaDelegado(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::AlIniciarSesionCompletada))
{
}

// ---------------------------------------------------------------- CREAR
void UMultiplayerSessionsSubsystem::CrearSesion(int32 NumConexionesPublicas, FString TipoPartida)
{
	UE_LOG(LogTemp, Warning, TEXT("[SesionesOW] CrearSesion: conexiones=%d tipo='%s' subsistema=%s"),
		NumConexionesPublicas, *TipoPartida, *NombreSubsistema());

	if (!EsValidaInterfazSesion())
	{
		UE_LOG(LogTemp, Error, TEXT("[SesionesOW] CrearSesion: NO hay interfaz de sesiones (OnlineSubsystem nulo)"));
		MultijugadorAlCrearSesionCompletada.Broadcast(false);
		return;
	}

	// FIX: si ya hay sesion, destruirla y CORTAR aca. La creacion sigue en AlDestruirSesionCompletada.
	if (InterfazSesion->GetNamedSession(NAME_GameSession) != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SesionesOW] CrearSesion: ya existe una sesion, se destruye primero"));
		bCrearSesionAlDestruir = true;
		UltimoNumConexionesPublicas = NumConexionesPublicas;
		UltimoTipoPartida = TipoPartida;
		DestruirSesion();
		return;
	}

	const FUniqueNetIdPtr IdJugador = ObtenerIdJugador(GetWorld());
	if (!IdJugador.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[SesionesOW] CrearSesion: el jugador local no tiene UniqueNetId (Steam no logueado?)"));
		MultijugadorAlCrearSesionCompletada.Broadcast(false);
		return;
	}

	CrearSesionCompletadaHandle = InterfazSesion->AddOnCreateSessionCompleteDelegate_Handle(CrearSesionCompletadaDelegado);

	UltimasConfiguracionesSesion = MakeShareable(new FOnlineSessionSettings());
	UltimasConfiguracionesSesion->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	UltimasConfiguracionesSesion->NumPublicConnections = NumConexionesPublicas;
	UltimasConfiguracionesSesion->bAllowJoinInProgress = true;
	UltimasConfiguracionesSesion->bAllowJoinViaPresence = true;
	UltimasConfiguracionesSesion->bShouldAdvertise = true;
	UltimasConfiguracionesSesion->bUsesPresence = true;
	UltimasConfiguracionesSesion->bUseLobbiesIfAvailable = true;
	UltimasConfiguracionesSesion->Set(FName("TipoPartida"), TipoPartida, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	UltimasConfiguracionesSesion->BuildUniqueId = 1;

	UE_LOG(LogTemp, Warning, TEXT("[SesionesOW] CrearSesion: llamando CreateSession (LAN=%d, lobbies=1, presence=1)"),
		UltimasConfiguracionesSesion->bIsLANMatch ? 1 : 0);

	if (!InterfazSesion->CreateSession(*IdJugador, NAME_GameSession, *UltimasConfiguracionesSesion))
	{
		UE_LOG(LogTemp, Error, TEXT("[SesionesOW] CrearSesion: CreateSession devolvio false"));
		InterfazSesion->ClearOnCreateSessionCompleteDelegate_Handle(CrearSesionCompletadaHandle);
		MultijugadorAlCrearSesionCompletada.Broadcast(false);
	}
}

// ---------------------------------------------------------------- BUSCAR
void UMultiplayerSessionsSubsystem::BuscarSesiones(int32 MaxResultadosBusqueda)
{
	UE_LOG(LogTemp, Warning, TEXT("[SesionesOW v2] BuscarSesiones: max=%d subsistema=%s"), MaxResultadosBusqueda, *NombreSubsistema());

	if (!EsValidaInterfazSesion())
	{
		UE_LOG(LogTemp, Error, TEXT("[SesionesOW v2] BuscarSesiones: NO hay interfaz de sesiones"));
		MultijugadorAlBuscarSesionesCompletado.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	const FUniqueNetIdPtr IdJugador = ObtenerIdJugador(GetWorld());
	if (!IdJugador.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[SesionesOW v2] BuscarSesiones: el jugador local no tiene UniqueNetId"));
		MultijugadorAlBuscarSesionesCompletado.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	if (UltimaBusquedaSesion.IsValid() && UltimaBusquedaSesion->SearchState == EOnlineAsyncTaskState::InProgress)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SesionesOW v2] BuscarSesiones: la busqueda anterior sigue EN CURSO (Steam solo admite una a la vez)"));
	}

	// Evitar handles duplicados si una busqueda anterior nunca llego a completar
	if (BuscarSesionesCompletadoHandle.IsValid())
	{
		InterfazSesion->ClearOnFindSessionsCompleteDelegate_Handle(BuscarSesionesCompletadoHandle);
	}
	BuscarSesionesCompletadoHandle = InterfazSesion->AddOnFindSessionsCompleteDelegate_Handle(BuscarSesionesCompletadoDelegado);

	UltimaBusquedaSesion = MakeShareable(new FOnlineSessionSearch());
	UltimaBusquedaSesion->MaxSearchResults = MaxResultadosBusqueda;
	UltimaBusquedaSesion->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	UltimaBusquedaSesion->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

	UE_LOG(LogTemp, Warning, TEXT("[SesionesOW v2] BuscarSesiones: LAN=%d SEARCH_LOBBIES=1 -> llamando FindSessions"),
		UltimaBusquedaSesion->bIsLanQuery ? 1 : 0);

	const bool bInicio = InterfazSesion->FindSessions(*IdJugador, UltimaBusquedaSesion.ToSharedRef());
	UE_LOG(LogTemp, Warning, TEXT("[SesionesOW v2] BuscarSesiones: FindSessions devolvio %d"), bInicio ? 1 : 0);

	if (!bInicio)
	{
		InterfazSesion->ClearOnFindSessionsCompleteDelegate_Handle(BuscarSesionesCompletadoHandle);
		MultijugadorAlBuscarSesionesCompletado.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

// ---------------------------------------------------------------- UNIRSE
void UMultiplayerSessionsSubsystem::UnirseASesion(const FOnlineSessionSearchResult& ResultadoSesion)
{
	UE_LOG(LogTemp, Warning, TEXT("[SesionesOW] UnirseASesion: host='%s' id=%s"),
		*ResultadoSesion.Session.OwningUserName, *ResultadoSesion.GetSessionIdStr());

	if (!EsValidaInterfazSesion())
	{
		UE_LOG(LogTemp, Error, TEXT("[SesionesOW] UnirseASesion: NO hay interfaz de sesiones"));
		MultijugadorAlUnirseASesionCompletado.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	// FIX: si ya estamos en una sesion (por ejemplo tras volver al menu), destruirla antes de unirse
	if (InterfazSesion->GetNamedSession(NAME_GameSession) != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SesionesOW] UnirseASesion: ya hay una sesion activa, se destruye antes de unirse"));
		bUnirseAlDestruir = true;
		ResultadoPendienteParaUnirse = ResultadoSesion;
		DestruirSesion();
		return;
	}

	IntentarUnirse(ResultadoSesion);
}

void UMultiplayerSessionsSubsystem::IntentarUnirse(const FOnlineSessionSearchResult& ResultadoSesion)
{
	const FUniqueNetIdPtr IdJugador = ObtenerIdJugador(GetWorld());
	if (!IdJugador.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[SesionesOW] IntentarUnirse: el jugador local no tiene UniqueNetId"));
		MultijugadorAlUnirseASesionCompletado.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	// Compatibilidad con UE 5.5+: el resultado debe indicar presence/lobbies para poder unirse
	FOnlineSessionSearchResult Copia = ResultadoSesion;
	Copia.Session.SessionSettings.bUsesPresence = true;
	Copia.Session.SessionSettings.bUseLobbiesIfAvailable = true;

	UnirseASesionCompletadoHandle = InterfazSesion->AddOnJoinSessionCompleteDelegate_Handle(UnirseASesionCompletadoDelegado);

	if (!InterfazSesion->JoinSession(*IdJugador, NAME_GameSession, Copia))
	{
		UE_LOG(LogTemp, Error, TEXT("[SesionesOW] IntentarUnirse: JoinSession devolvio false"));
		InterfazSesion->ClearOnJoinSessionCompleteDelegate_Handle(UnirseASesionCompletadoHandle);
		MultijugadorAlUnirseASesionCompletado.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

// ---------------------------------------------------------------- DESTRUIR / INICIAR
void UMultiplayerSessionsSubsystem::DestruirSesion()
{
	if (!EsValidaInterfazSesion())
	{
		MultijugadorAlDestruirSesionCompletada.Broadcast(false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[SesionesOW] DestruirSesion"));
	DestruirSesionCompletadaHandle = InterfazSesion->AddOnDestroySessionCompleteDelegate_Handle(DestruirSesionCompletadaDelegado);

	if (!InterfazSesion->DestroySession(NAME_GameSession))
	{
		UE_LOG(LogTemp, Error, TEXT("[SesionesOW] DestruirSesion: DestroySession devolvio false"));
		InterfazSesion->ClearOnDestroySessionCompleteDelegate_Handle(DestruirSesionCompletadaHandle);
		bCrearSesionAlDestruir = false;
		bUnirseAlDestruir = false;
		MultijugadorAlDestruirSesionCompletada.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::IniciarSesion()
{
	// (vacio, igual que el original)
}

bool UMultiplayerSessionsSubsystem::EsValidaInterfazSesion()
{
	if (!InterfazSesion)
	{
		IOnlineSubsystem* Subsistema = IOnlineSubsystem::Get();
		if (Subsistema)
		{
			InterfazSesion = Subsistema->GetSessionInterface();
		}
	}
	return InterfazSesion.IsValid();
}

// ---------------------------------------------------------------- CALLBACKS
void UMultiplayerSessionsSubsystem::AlCrearSesionCompletada(FName NombreSesion, bool bFueExitoso)
{
	UE_LOG(LogTemp, Warning, TEXT("[SesionesOW] AlCrearSesionCompletada: sesion=%s exito=%d"), *NombreSesion.ToString(), bFueExitoso ? 1 : 0);

	if (InterfazSesion)
	{
		InterfazSesion->ClearOnCreateSessionCompleteDelegate_Handle(CrearSesionCompletadaHandle);
	}

	MultijugadorAlCrearSesionCompletada.Broadcast(bFueExitoso);
}

void UMultiplayerSessionsSubsystem::AlBuscarSesionesCompletado(bool bFueExitoso)
{
	if (InterfazSesion)
	{
		InterfazSesion->ClearOnFindSessionsCompleteDelegate_Handle(BuscarSesionesCompletadoHandle);
		BuscarSesionesCompletadoHandle.Reset();
	}

	const int32 Cantidad = UltimaBusquedaSesion.IsValid() ? UltimaBusquedaSesion->SearchResults.Num() : 0;
	UE_LOG(LogTemp, Warning, TEXT("[SesionesOW v2] AlBuscarSesionesCompletado: exito=%d resultados=%d"), bFueExitoso ? 1 : 0, Cantidad);

	if (Cantidad <= 0)
	{
		MultijugadorAlBuscarSesionesCompletado.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	for (const FOnlineSessionSearchResult& R : UltimaBusquedaSesion->SearchResults)
	{
		FString Tipo = TEXT("(sin TipoPartida)");
		R.Session.SessionSettings.Get(FName("TipoPartida"), Tipo);
		UE_LOG(LogTemp, Warning, TEXT("[SesionesOW v2]   resultado: host='%s' tipo='%s' libres=%d ping=%d id=%s"),
			*R.Session.OwningUserName, *Tipo, R.Session.NumOpenPublicConnections, R.PingInMs, *R.GetSessionIdStr());
	}

	MultijugadorAlBuscarSesionesCompletado.Broadcast(UltimaBusquedaSesion->SearchResults, bFueExitoso);
}

void UMultiplayerSessionsSubsystem::AlUnirseASesionCompletado(FName NombreSesion, EOnJoinSessionCompleteResult::Type Resultado)
{
	UE_LOG(LogTemp, Warning, TEXT("[SesionesOW] AlUnirseASesionCompletado: sesion=%s resultado=%d (0=Success)"), *NombreSesion.ToString(), static_cast<int32>(Resultado));

	if (InterfazSesion)
	{
		InterfazSesion->ClearOnJoinSessionCompleteDelegate_Handle(UnirseASesionCompletadoHandle);
	}

	MultijugadorAlUnirseASesionCompletado.Broadcast(Resultado);
}

void UMultiplayerSessionsSubsystem::AlDestruirSesionCompletada(FName NombreSesion, bool bFueExitoso)
{
	UE_LOG(LogTemp, Warning, TEXT("[SesionesOW] AlDestruirSesionCompletada: exito=%d crearDespues=%d unirseDespues=%d"),
		bFueExitoso ? 1 : 0, bCrearSesionAlDestruir ? 1 : 0, bUnirseAlDestruir ? 1 : 0);

	if (InterfazSesion)
	{
		InterfazSesion->ClearOnDestroySessionCompleteDelegate_Handle(DestruirSesionCompletadaHandle);
	}

	if (bCrearSesionAlDestruir)
	{
		bCrearSesionAlDestruir = false;
		if (bFueExitoso)
		{
			CrearSesion(UltimoNumConexionesPublicas, UltimoTipoPartida);
		}
		else
		{
			MultijugadorAlCrearSesionCompletada.Broadcast(false);
		}
	}

	if (bUnirseAlDestruir)
	{
		bUnirseAlDestruir = false;
		if (bFueExitoso)
		{
			IntentarUnirse(ResultadoPendienteParaUnirse);
		}
		else
		{
			MultijugadorAlUnirseASesionCompletado.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		}
	}

	MultijugadorAlDestruirSesionCompletada.Broadcast(bFueExitoso);
}

void UMultiplayerSessionsSubsystem::AlIniciarSesionCompletada(FName NombreSesion, bool bFueExitoso)
{
	// (vacio, igual que el original)
}
