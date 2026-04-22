// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROYECTO404TNF_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	//Properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHealth = 100.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ActualHealth = MaxHealth;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsDead = false;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//Getters
	UFUNCTION(BlueprintCallable)
	float GetHealth(){return ActualHealth;}
	
	UFUNCTION(BlueprintCallable)
	bool GetIsDead(){return bIsDead;}
	
	// Functions
	UFUNCTION(BlueprintCallable)
	void HandleDamage(float Damage);
	
	UFUNCTION(BlueprintCallable)
	void HandleHealth(float Heal);
	
	UFUNCTION(BlueprintCallable)
	void HandleDeath();
	
	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;
};
