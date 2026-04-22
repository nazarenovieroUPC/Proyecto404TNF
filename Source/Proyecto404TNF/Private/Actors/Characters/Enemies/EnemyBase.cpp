// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/EnemyBase.h"
#include "Components/HealthComponent.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//Getters
float AEnemyBase::GetActualHealth_Implementation()
{
	if (!HealthComponent) return 0.0f;
	return HealthComponent->ActualHealth;
}

float AEnemyBase::GetMaxHealth_Implementation()
{
	if (!HealthComponent) return 0.0f;
	return HealthComponent->MaxHealth;
}

bool AEnemyBase::GetIsDead_Implementation()
{
	if (!HealthComponent) return false;
	return HealthComponent->bIsDead;
}

//Interface
void AEnemyBase::TakeDamage_Implementation(float Damage)
{
	IDamageableInterface::TakeDamage_Implementation(Damage);
	if (HealthComponent)
	{
		HealthComponent -> HandleDamage(Damage);
	}
}

