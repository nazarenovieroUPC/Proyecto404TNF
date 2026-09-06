// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Managers/HordeManager.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Structures/HordeDataStruct.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AHordeManager::AHordeManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentWaveIndex = 0;
	EnemiesSpawnedInCurrentWave = 0;
}

// Called when the game starts or when spawned
void AHordeManager::BeginPlay()
{
	Super::BeginPlay();
	
	StartHordeSystem();
}

// Called every frame
void AHordeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHordeManager::StartHordeSystem()
{
	if (Waves.Num() > 0)
	{
		CurrentWaveIndex = 0;
		StartWave();
	}
}

void AHordeManager::StartWave()
{
	if (!Waves.IsValidIndex(CurrentWaveIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("¡Todas las oleadas han sido completadas!"));
		return;
	}

	FHordeWave CurrentWave = Waves[CurrentWaveIndex];
	EnemiesSpawnedInCurrentWave = 0;

	UE_LOG(LogTemp, Warning, TEXT("Iniciando Oleada %d"), CurrentWaveIndex + 1);

	GetWorld()->GetTimerManager().SetTimer(
		WaveTimerHandle, 
		this, 
		&AHordeManager::EndWave, 
		CurrentWave.WaveDuration, 
		false
	);

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle, 
		this, 
		&AHordeManager::SpawnEnemy, 
		CurrentWave.SpawnInterval, 
		true, 
		0.0f
	);
}

void AHordeManager::EndWave()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("Oleada %d terminada por tiempo."), CurrentWaveIndex + 1);

	CurrentWaveIndex++;

	StartWave();
}

void AHordeManager::SpawnEnemy()
{
	FHordeWave CurrentWave = Waves[CurrentWaveIndex];
	
	if (EnemiesSpawnedInCurrentWave >= CurrentWave.EnemiesToSpawn)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}
	
	if (SpawnPoints.Num() > 0 && CurrentWave.EnemyClass != nullptr)
	{
		int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		AActor* SpawnLocationActor = SpawnPoints[RandomIndex];

		FVector SpawnLocation = SpawnLocationActor->GetActorLocation();
		FRotator SpawnRotation = SpawnLocationActor->GetActorRotation();
		
		APawn* SpawnedEnemy = GetWorld()->SpawnActor<APawn>(CurrentWave.EnemyClass, SpawnLocation, SpawnRotation);
		
		if (SpawnedEnemy)
		{
			AAIController* AIController = Cast<AAIController>(SpawnedEnemy->GetController());
            
			if (AIController)
			{
				APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
				
				UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
				
				if (PlayerPawn && BlackboardComp) BlackboardComp->SetValueAsObject(FName("TargetActor"), PlayerPawn);
        
				EnemiesSpawnedInCurrentWave++;
			}
		}
	}
}
