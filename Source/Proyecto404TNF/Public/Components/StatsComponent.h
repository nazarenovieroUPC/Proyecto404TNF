// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structures/CharacterStatsStruct.h"
#include "StatsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatsComponent();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
	TObjectPtr<UDataTable> StatsDataTable;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
	FName LevelRowID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	FCharacterStatsRow StatsBase;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void ChangeSpeedMovement(float Speed);
	
	UFUNCTION(BlueprintCallable)
	void ChangeLevelRowID(int32 ID);
	
	UFUNCTION(BlueprintCallable)
	void OnBuff();
};
