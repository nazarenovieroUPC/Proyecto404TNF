// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HordeManager.generated.h"

struct FHordeWave;

UCLASS()
class PROYECTO404TNF_API AHordeManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHordeManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;

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
