// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/LevelingComponent.h"
#include "Net/UnrealNetwork.h"

ULevelingComponent::ULevelingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    
    SetIsReplicatedByDefault(true);

    CurrentLevel = 1;
    CurrentXP = 0.0f;
    MaxXP = 100.0f;
    XPRequirementMultiplier = 1.5f;
}

void ULevelingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ULevelingComponent, CurrentLevel);
    DOREPLIFETIME(ULevelingComponent, CurrentXP);
    DOREPLIFETIME(ULevelingComponent, MaxXP);
    DOREPLIFETIME(ULevelingComponent, XPRequirementMultiplier);
}

void ULevelingComponent::BeginPlay()
{
    Super::BeginPlay();
    
    OnExperienceChanged.Broadcast(CurrentXP, MaxXP);
}

void ULevelingComponent::AddExperience(float XPToAdd)
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || XPToAdd <= 0.0f)
    {
        return;
    }

    CurrentXP += XPToAdd;

    CheckLevelUp();
    
    OnExperienceChanged.Broadcast(CurrentXP, MaxXP);
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
}

void ULevelingComponent::OnRep_Experience()
{
    OnExperienceChanged.Broadcast(CurrentXP, MaxXP);
}

void ULevelingComponent::OnRep_CurrentLevel(int32 OldLevel)
{
    OnLevelUp.Broadcast(CurrentLevel);
}