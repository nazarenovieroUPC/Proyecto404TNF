// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatsComponent.h"

#include "Components/HealthComponent.h"
#include "Components/LevelingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Structures/CharacterStatsStruct.h"


// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if ( StatsDataTable && !LevelRowID.IsNone())
	{
		FCharacterStatsRow* Row = StatsDataTable -> FindRow<FCharacterStatsRow>(LevelRowID, TEXT(""));
		if (Row){ StatsBase = *Row; }
	
	
		ULevelingComponent* LevelComponent = GetOwner()->FindComponentByClass<ULevelingComponent>();
		if (LevelComponent){ LevelComponent->OnLevelUp.AddDynamic(this, &UStatsComponent::ChangeLevelRowID); }
	}
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::ChangeSpeedMovement(float Speed)
{
	UCharacterMovementComponent* CharacterMovement = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	if (CharacterMovement){ CharacterMovement -> MaxWalkSpeed = Speed; }
}

void UStatsComponent::ChangeLevelRowID(int32 ID)
{
	FCharacterStatsRow* Row = StatsDataTable -> FindRow<FCharacterStatsRow>(FName(*FString::FromInt(ID)), TEXT(""));
	if (Row){ StatsBase = *Row; }
	
	ChangeSpeedMovement(StatsBase.Speed);
}

void UStatsComponent::OnBuff()
{
}

