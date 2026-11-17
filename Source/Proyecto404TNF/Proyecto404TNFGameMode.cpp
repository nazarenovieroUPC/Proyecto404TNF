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
	
	if (JugadoresConectados > 1)
	{
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
				}
			}
		}
	}
}
