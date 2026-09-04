// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatsComponent.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROYECTO404TNF_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	//Properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;
	
	UPROPERTY(ReplicatedUsing = OnRep_ActualHealth, BlueprintReadWrite, EditAnywhere, Category = "Health")
	float ActualHealth = MaxHealth;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	bool bIsDead = false;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateMaxHealth(float NewMaxHealth);
	
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
	void HandleHeal(float Heal);
	
	UFUNCTION(BlueprintCallable)
	void HandleDeath();
	
	UFUNCTION()
	void OnRep_ActualHealth();
	
	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;
	
	UPROPERTY(BlueprintAssignable, Category = "Health/Events")
	FOnHealthChanged OnHealthChanged;
};
