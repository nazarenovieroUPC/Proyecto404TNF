// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LootComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API ULootComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	ULootComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootComponent")
	float XPToDrop;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootComponent")
	//TArray<TSubclassOf<class AItemBase>> ItemsDrop;
	
	UFUNCTION(BlueprintCallable, Category = "LootComponent")
	void DropLoot();

protected:
	
	virtual void BeginPlay() override;

public:
		
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
