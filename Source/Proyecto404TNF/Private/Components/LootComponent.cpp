// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LootComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LevelingComponent.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
ULootComponent::ULootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	XPToDrop = 50.0f;
	// ...
}


void ULootComponent::DropLoot()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter)
	{
		ULevelingComponent* PlayerLeveling = PlayerCharacter->FindComponentByClass<ULevelingComponent>();
		if (PlayerLeveling)
		{
			PlayerLeveling->AddExperience(XPToDrop);
		}
	}
}

// Called when the game starts
void ULootComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

