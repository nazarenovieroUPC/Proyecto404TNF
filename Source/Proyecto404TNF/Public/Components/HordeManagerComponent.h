// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HordeManagerComponent.generated.h"

struct FHordeWave;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UHordeManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHordeManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horde System")
	TArray<FHordeWave> Waves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horde System")
	TArray<AActor*> SpawnPoints;

	UFUNCTION(BlueprintCallable, Category = "Horde System")
	void StartHordeSystem();

private:
	int32 CurrentWaveIndex;
	int EnemiesSpawnedInCurrentWave;
	
	FTimerHandle WaveTimerHandle;
	FTimerHandle SpawnTimerHandle;
	
	void StartWave();
	void EndWave();
	void SpawnEnemy();
};
