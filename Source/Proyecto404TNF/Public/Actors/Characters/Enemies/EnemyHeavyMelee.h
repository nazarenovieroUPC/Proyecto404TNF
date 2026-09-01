// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyMelee.h"
#include "EnemyHeavyMelee.generated.h"

class UBoxComponent;

UCLASS()
class PROYECTO404TNF_API AEnemyHeavyMelee : public AEnemyMelee
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyHeavyMelee();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	TObjectPtr<UCapsuleComponent> ChargeHitBox;
	
	//Charge Skill
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Skills")
	float CooldownCharge = 10.f;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Skills")
	float KnockbackForceCharge = 1000.f;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Skills")
	float SpeedCharge = 1200.f;
	
	bool bIsCharging = false;
	FTimerHandle ChargeTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(Blueprintable, Category = "Skills")
	void StartCharge();
	
	void StopCharge();
	
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void OnChargeHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
					 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
					 bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
				   UPrimitiveComponent* OtherComp, FVector NormalImpulse, 
				   const FHitResult& Hit);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void EnemyAttack_Implementation() override;
};
