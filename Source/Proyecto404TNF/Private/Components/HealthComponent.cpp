// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



void UHealthComponent::GetDamaged(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	if (Health <= 0.f)
	{
		TObjectPtr<AActor> Owner = GetOwner();
		if (Owner){
			Owner->Destroy();
		}
	}
}

float UHealthComponent::GetHealth()
{
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	return Health;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

