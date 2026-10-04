// Copyright Epic Games, Inc. All Rights Reserved.

#include "Proyecto404TNFGameMode.h"
#include "Proyecto404TNFCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "GameFramework/GameStateBase.h" 
#include "GameFramework/PlayerState.h"

AProyecto404TNFGameMode::AProyecto404TNFGameMode()
{
	// set default pawn class to our Blueprinted character
}

void AProyecto404TNFGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	FTimerHandle TimerHandle_AsignarRoles;
	GetWorldTimerManager().SetTimer(TimerHandle_AsignarRoles, this, &AProyecto404TNFGameMode::AsignarRolesCoop, 2.0f, false);
}

void AProyecto404TNFGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void AProyecto404TNFGameMode::AsignarRolesCoop()
{
	int32 JugadoresConectados = GetNumPlayers();
    
	// Chivato 1: Verificamos si la función se llamó y cuántos jugadores detectó
	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Yellow, FString::Printf(TEXT("GameMode: Ejecutando AsignarRoles. Jugadores detectados: %d"), JugadoresConectados));

	if (JugadoresConectados > 1)
	{
		// Forma más segura de buscar controladores en el servidor
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PC = Iterator->Get();
			if (PC)
			{
				AProyecto404TNFCharacter* Personaje = Cast<AProyecto404TNFCharacter>(PC->GetPawn());
				if (Personaje)
				{
					int32 RolAleatorio = FMath::RandRange(0, 2);
					ERolCoop RolAsignado = static_cast<ERolCoop>(RolAleatorio);

					Personaje->Multicast_RecibirBuffCoop(RolAsignado);
                    
					// Chivato 2: Éxito
					GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, TEXT("GameMode: ¡Buff enviado al personaje con éxito!"));
				}
				else
				{
					// Chivato 3: Fallo crítico de Pawn
					GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, TEXT("GameMode Error: El controlador está conectado, pero NO tiene un Pawn físico en el mapa."));
				}
			}
		}
	}
}
