// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/StatsComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

//FUNCTIONS
void UHealthComponent::HandleDamage(float Damage)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	ActualHealth = FMath::Clamp(ActualHealth - Damage, 0.f, MaxHealth);
	
	HandleDeath();
	OnHealthChanged.Broadcast(ActualHealth, MaxHealth);
}

void UHealthComponent::HandleHeal(float Heal)
{
	ActualHealth = FMath::Clamp(ActualHealth + Heal, 0.f, MaxHealth);
}

void UHealthComponent::HandleDeath()
{
	if (ActualHealth <= 0.f)
	{
		bIsDead = true;
		OnDeath.Broadcast();
	}
}

void UHealthComponent::OnRep_ActualHealth()
{
	OnHealthChanged.Broadcast(ActualHealth, MaxHealth);
}

void UHealthComponent::UpdateMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	if (ActualHealth > MaxHealth)
	{
		ActualHealth = MaxHealth;
	}
	OnHealthChanged.Broadcast(ActualHealth, MaxHealth);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
	//Setea la vida si el actor contiene el StatComponent
	UStatsComponent* StatsComponent = GetOwner()->FindComponentByClass<UStatsComponent>();
	
	if (StatsComponent)
	{
		MaxHealth = StatsComponent->StatsBase.MaxHealth;
		ActualHealth = MaxHealth;
	}
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UHealthComponent, ActualHealth);
}
