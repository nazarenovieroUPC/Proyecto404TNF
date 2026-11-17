// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyMelee.generated.h"

UCLASS()
class PROYECTO404TNF_API AEnemyMelee : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyMelee();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attack")
	bool bIsAttacking = false;

	// Poné acá lo que dura la animación de ataque (en segundos)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float AttackDuration = 1.4f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FTimerHandle AttackTimerHandle;

	void EndAttack();

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void EnemyAttack_Implementation() override;
};
