// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageFlashComponent.generated.h"

class UMaterialInstanceDynamic;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UDamageFlashComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDamageFlashComponent();

	UFUNCTION(BlueprintCallable, Category = "Damage Flash")
	void TriggerFlash();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Flash")
	float WhiteDuration = 0.08f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Flash")
	float RedDuration = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Flash")
	float FlashIntensity = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Flash")
	FName ColorParamName = TEXT("FlashColor");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Flash")
	FName AmountParamName = TEXT("FlashAmount");
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayFlash();
	
private:
	void SetFlash(const FLinearColor& Color, float Amount);

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;

	float Elapsed = 0.f;
};
