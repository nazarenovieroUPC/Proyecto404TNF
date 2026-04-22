// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageableInterface.h"
#include "EnemyBase.generated.h"

class UHealthComponent;

UCLASS()
class PROYECTO404TNF_API AEnemyBase : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Damageable Interface
	virtual float GetActualHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;
	virtual bool GetIsDead_Implementation() override;
	virtual void TakeDamage_Implementation(float Damage) override;
	
	//Health Component
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;
};
