// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/StatsComponent.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    SetIsReplicatedByDefault(true);

    MaxHealth = 100.0f;
    ActualHealth = MaxHealth;
    bIsDead = false;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(UHealthComponent, MaxHealth);
    DOREPLIFETIME(UHealthComponent, ActualHealth);
    DOREPLIFETIME(UHealthComponent, bIsDead);
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        if (UStatsComponent* StatsComponent = GetOwner()->FindComponentByClass<UStatsComponent>())
        {
            MaxHealth = StatsComponent->StatsBase.MaxHealth;
            ActualHealth = MaxHealth;
        }
    }
    
    OnHealthChanged.Broadcast(ActualHealth, MaxHealth);
}

void UHealthComponent::HandleDamage(float Damage)
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || bIsDead || Damage <= 0.0f)
    {
        return;
    }
    
    ActualHealth = FMath::Clamp(ActualHealth - Damage, 0.0f, MaxHealth);
    
    if (ActualHealth <= 0.0f)
    {
        HandleDeath();
    }
    
    OnHealthChanged.Broadcast(ActualHealth, MaxHealth);
}

void UHealthComponent::HandleHeal(float Heal)
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || bIsDead || Heal <= 0.0f)
    {
        return;
    }
    
    ActualHealth = FMath::Clamp(ActualHealth + Heal, 0.0f, MaxHealth);
    
    OnHealthChanged.Broadcast(ActualHealth, MaxHealth);
}

void UHealthComponent::HandleDeath()
{
    if (bIsDead) return;

    bIsDead = true;
    OnDeath.Broadcast(); 
}

void UHealthComponent::UpdateMaxHealth(float NewMaxHealth)
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || NewMaxHealth <= 0.0f)
    {
        return;
    }
    
    MaxHealth = NewMaxHealth;
    
    if (ActualHealth > MaxHealth)
    {
        ActualHealth = MaxHealth;
    }
    
    OnHealthChanged.Broadcast(ActualHealth, MaxHealth);
}

void UHealthComponent::OnRep_ActualHealth()
{
    OnHealthChanged.Broadcast(ActualHealth, MaxHealth);
}

void UHealthComponent::OnRep_MaxHealth()
{
    OnHealthChanged.Broadcast(ActualHealth, MaxHealth);
}

void UHealthComponent::OnRep_IsDead()
{
    if (bIsDead)
    {
        OnDeath.Broadcast();
    }
}