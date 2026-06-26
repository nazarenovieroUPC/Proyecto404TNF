// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AProjectileBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMagicAttack);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Combat)
	TSubclassOf<AProjectileBase> Projectile;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Combat)
	bool bCanMagicAttack = true;
	
	//Delegates
	UPROPERTY(BlueprintAssignable, Category = Combat)
	FOnMagicAttack OnMagicAttack;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	FTimerHandle MagicTimerHandle;

public:
	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void MeleeAttack(float Damage);
	
	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void MagicAttack(float MagicDamage, float MagicCoolDown);
	
	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void RangedAttack(float Damage);
};
