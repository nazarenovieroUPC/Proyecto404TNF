// Copyright Epic Games, Inc. All Rights Reserved.

#include "Proyecto404TNFGameMode.h"
#include "Proyecto404TNFCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProyecto404TNFGameMode::AProyecto404TNFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
