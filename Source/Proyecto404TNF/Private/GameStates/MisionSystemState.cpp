// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/MisionSystemState.h"
#include "Net/UnrealNetwork.h"

AMisionSystemState::AMisionSystemState()
{
	ItemsRecolectados = 0;
	ItemsNecesarios = 10;
}

void AMisionSystemState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMisionSystemState, ItemsRecolectados);
}

void AMisionSystemState::AgregarItemMision()
{
	if (HasAuthority())
	{
		ItemsRecolectados++;
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Items: %d / %d"), ItemsRecolectados, ItemsNecesarios));
		}
		
		if (ItemsRecolectados >= ItemsNecesarios)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("MISION COMPLETADA: Hora de armar el puente!"));
			}
		}
	}
}
