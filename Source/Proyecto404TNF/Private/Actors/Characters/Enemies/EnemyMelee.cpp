// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/EnemyMelee.h"

#include "Components/CombatComponent.h"
#include "Components/StatsComponent.h"

// Sets default values
AEnemyMelee::AEnemyMelee()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemyMelee::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AEnemyMelee::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyMelee::EnemyAttack_Implementation()
{
	Super::EnemyAttack_Implementation();
	
	if (CombatComponent) CombatComponent -> MeleeAttack(StatsComponent->StatsTotal.MeleeDamage);
}

