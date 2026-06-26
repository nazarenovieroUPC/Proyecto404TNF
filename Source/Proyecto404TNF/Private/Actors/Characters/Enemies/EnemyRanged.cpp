// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/EnemyRanged.h"

#include "Components/CombatComponent.h"
#include "Components/StatsComponent.h"


// Sets default values
AEnemyRanged::AEnemyRanged()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemyRanged::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AEnemyRanged::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyRanged::EnemyAttack_Implementation()
{
	// Cambiar Stats de enemigo 
	if (CombatComponent) CombatComponent -> RangedAttack(StatsComponent->StatsTotal.MagicDamage);
}

