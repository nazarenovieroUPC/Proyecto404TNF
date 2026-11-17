// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/EnemyMelee.h"

#include "Components/CombatComponent.h"
#include "Components/StatsComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEnemyMelee::AEnemyMelee()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyMelee::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AEnemyMelee, bIsAttacking);
}

// Called when the game starts or when spawned
void AEnemyMelee::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyMelee::EndAttack()
{
	bIsAttacking = false;
}

// Called to bind functionality to input
void AEnemyMelee::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyMelee::EnemyAttack_Implementation()
{
	Super::EnemyAttack_Implementation();
	
	if (!HasAuthority() || bIsAttacking) return;

	bIsAttacking = true;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyMelee::EndAttack, AttackDuration, false);

	if (CombatComponent) CombatComponent->MeleeAttack(StatsComponent->StatsTotal.MeleeDamage);
}

