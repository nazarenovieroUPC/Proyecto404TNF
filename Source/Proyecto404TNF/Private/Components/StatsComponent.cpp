// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StatsComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/EquipmentComponent.h"
#include "Components/LevelingComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DataTable.h"

UStatsComponent::UStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UStatsComponent, StatsTotal);
    DOREPLIFETIME(UStatsComponent, StatsBase);
    DOREPLIFETIME(UStatsComponent, StatsBuff);
}

void UStatsComponent::BeginPlay()
{
    Super::BeginPlay();
    
    if (StatsDataTable && !LevelRowID.IsNone())
    {
        FCharacterStatsRow* Row = StatsDataTable->FindRow<FCharacterStatsRow>(LevelRowID, TEXT("StatsInit"));
        if (Row)
        {
            StatsBase = *Row;
            StatsTotal = StatsBase; 
        }
    }
    
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        if (ULevelingComponent* LevelComponent = GetOwner()->FindComponentByClass<ULevelingComponent>())
        {
            LevelComponent->OnLevelUp.AddDynamic(this, &UStatsComponent::ChangeLevelRowID);
        }

        RecalculateTotalStats();
    }
}

void UStatsComponent::ChangeSpeedMovement(float Speed)
{
    if (ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner()))
    {
        if (UCharacterMovementComponent* MoveComp = CharacterOwner->GetCharacterMovement())
        {
            MoveComp->MaxWalkSpeed = Speed;
        }
    }
}

void UStatsComponent::ChangeLevelRowID(int32 ID)
{
    if (!GetOwner() || !GetOwner()->HasAuthority()) return;

    if (StatsDataTable)
    {
        FCharacterStatsRow* Row = StatsDataTable->FindRow<FCharacterStatsRow>(FName(*FString::FromInt(ID)), TEXT("StatsLevelUp"));
        if (Row)
        {
            StatsBase = *Row;
        }
    }

    RecalculateTotalStats();
}

void UStatsComponent::OnBuff()
{
    // Función reservada para disparar efectos visuales o timers de buffs si se requiere
}

void UStatsComponent::InitializeEquipmentLink(UEquipmentComponent* EquipComp)
{
    CachedEquipment = EquipComp;
    if (CachedEquipment)
    {
        CachedEquipment->OnStatsUpdated.AddDynamic(this, &UStatsComponent::RecalculateTotalStats);
    }

    if (GetOwner() && GetOwner()->HasAuthority())
    {
        RecalculateTotalStats();
    }
}

void UStatsComponent::RecalculateTotalStats()
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        return;
    }

    StatsTotal = StatsBase;
    
    StatsTotal.MaxHealth += StatsBuff.MaxHealth;
    StatsTotal.Defense += StatsBuff.Defense;
    StatsTotal.MeleeDamage += StatsBuff.MeleeDamage;
    StatsTotal.MagicDamage += StatsBuff.MagicDamage;
    StatsTotal.Speed += StatsBuff.Speed;
    StatsTotal.MagicCoolDown -= StatsBuff.MagicCoolDown;
    
    if (CachedEquipment)
    {
        StatsTotal.MaxHealth += CachedEquipment->GetStatBonus(EItemStatType::Health);
        StatsTotal.Defense += CachedEquipment->GetStatBonus(EItemStatType::Defense);
        StatsTotal.MeleeDamage += CachedEquipment->GetStatBonus(EItemStatType::MeleeDamage);
        StatsTotal.MagicDamage += CachedEquipment->GetStatBonus(EItemStatType::MagicDamage);
        StatsTotal.Speed += CachedEquipment->GetStatBonus(EItemStatType::Speed);
        StatsTotal.MagicCoolDown -= CachedEquipment->GetStatBonus(EItemStatType::MagicCoolDown);
    }
    
    ChangeSpeedMovement(StatsTotal.Speed);
    
    OnTotalStatsUpdated.Broadcast();
}

void UStatsComponent::OnRep_StatsTotal()
{
    ChangeSpeedMovement(StatsTotal.Speed);
    
    OnTotalStatsUpdated.Broadcast();
}