// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageableInterface.h"
#include "Interfaces/EnemyInterface.h"
#include "EnemyBase.generated.h"

class UBehaviorTree;
class UStatsComponent;
class UCombatComponent;
class UHealthComponent;
class ULootComponent;

UCLASS()
class PROYECTO404TNF_API AEnemyBase : public ACharacter, public IDamageableInterface, public IEnemyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();
	
	// Components
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<ULootComponent> LootComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCombatComponent> CombatComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UStatsComponent> StatsComponent;
	
	// Varaibles
	UPROPERTY(Blueprintable, EditAnywhere, BlueprintReadWrite, Category = "Behavior Tree")
	TObjectPtr<UBehaviorTree> EnemyBehaviorTree;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bHasDroppedLoot;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Damageable Interface
	virtual float GetActualHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;
	virtual bool GetIsDead_Implementation() override;
	virtual void TakeDamage_Implementation(float Damage, AActor* DamagerActor) override;
	
	//Enemy Interface
	virtual void EnemyAttack_Implementation() override;
	
	virtual float SetMovementSpeed_Implementation(EMovementSpeed MovementSpeed) override;
};
