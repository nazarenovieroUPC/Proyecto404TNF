// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LevelingComponent.h"


// Sets default values for this component's properties
ULevelingComponent::ULevelingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	CurrentLevel = 1;
	CurrentXP = 0.0f;
	MaxXP = 100.0f;
	XPRequirementMultiplier = 1.5f;
}


// Called when the game starts
void ULevelingComponent::BeginPlay()
{
	Super::BeginPlay();

	OnExperienceChanged.Broadcast(CurrentXP, MaxXP);
}

void ULevelingComponent::AddExperience(float XPToAdd)
{
	if (XPToAdd <= 0.0f) return;
	
	CurrentXP += XPToAdd;
	
	OnExperienceChanged.Broadcast(CurrentXP, MaxXP);
	
	CheckLevelUp();
}

void ULevelingComponent::CheckLevelUp()
{
	while (CurrentXP >= MaxXP)
	{
		CurrentXP -= MaxXP;
		CurrentLevel++;
		
		MaxXP *= XPRequirementMultiplier;
		OnLevelUp.Broadcast(CurrentLevel);
	}
	OnExperienceChanged.Broadcast(CurrentXP, MaxXP);
}


