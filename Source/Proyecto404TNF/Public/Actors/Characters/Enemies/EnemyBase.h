// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageableInterface.h"
#include "Interfaces/EnemyInterface.h"
#include "EnemyBase.generated.h"

class UDamageFlashComponent;
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UDamageFlashComponent> DamageFlashComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> DeathSound;
	
	// Variables
	UPROPERTY(Blueprintable, EditAnywhere, BlueprintReadWrite, Category = "Behavior Tree")
	TObjectPtr<UBehaviorTree> EnemyBehaviorTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
	float RagdollLifeSpan = 4.f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//WalkSpeeds
	UPROPERTY(Blueprintable, EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int WalkSpeedIdle = 0;
	
	UPROPERTY(Blueprintable, EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int WalkSpeedPatrol = 200;
	
	UPROPERTY(Blueprintable, EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int WalkSpeedChase = 800;
	
	UPROPERTY(ReplicatedUsing = OnRep_IsDead, BlueprintReadOnly, Category = "Death")
	bool bIsDead = false;

	bool bHasDroppedLoot;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_IsDead();
	
	UFUNCTION(BlueprintCallable, Category = "Death")
	virtual void Die();

	virtual void EnterRagdoll();
	
	//Damageable Interface
	virtual float GetActualHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;
	virtual bool GetIsDead_Implementation() override;
	
	virtual void TakeDamage_Implementation(float Damage, AActor* DamagerActor) override;
	
	//Enemy Interface
	virtual void EnemyAttack_Implementation() override;
	
	virtual float SetMovementSpeed_Implementation(EMovementSpeed MovementSpeed) override;
};
