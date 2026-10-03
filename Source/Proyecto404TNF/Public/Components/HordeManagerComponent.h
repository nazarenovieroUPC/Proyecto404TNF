// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HordeManagerComponent.generated.h"

struct FHordeWave;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHordeVictory);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UHordeManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHordeManagerComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "Horde System|Events")
	FOnHordeVictory OnHordeVictory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horde System")
	TArray<FHordeWave> Waves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horde System")
	TArray<AActor*> SpawnPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horde System|Time")
	float TotalSurvivalTime = 300.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Horde System")
	void StartHordeSystem();

private:
	int32 CurrentWaveIndex;
	int EnemiesSpawnedInCurrentWave;
	int32 EnemiesAlive;
	
	bool bTimeIsUp;
	
	FTimerHandle SpawnTimerHandle;
	FTimerHandle GlobalTimerHandle;
	
	void StartWave();
	void SpawnEnemy();
	void TimeLimitReached();
	void CheckWaveState();
	
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
};
