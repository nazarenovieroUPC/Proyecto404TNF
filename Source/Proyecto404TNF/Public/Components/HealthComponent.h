// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROYECTO404TNF_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health = 100;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHealth = 100.f;
	
	UFUNCTION(BlueprintCallable)
	void GetDamaged(float Damage);
	
	UFUNCTION(BlueprintCallable)
	float GetHealth();
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
