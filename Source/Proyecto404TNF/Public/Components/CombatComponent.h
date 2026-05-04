// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MagicProjectile.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Combat)
	TSubclassOf<AMagicProjectile> MagicProjectile;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void MeleeAttack(float Damage);
	
	UFUNCTION(BlueprintCallable, Category = Combat)
	virtual void MagicAttack(float MagicDamage);
};
