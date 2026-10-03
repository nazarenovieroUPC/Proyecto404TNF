// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HordeManagerComponent.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Structures/HordeDataStruct.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values for this component's properties
UHordeManagerComponent::UHordeManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHordeManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHordeManagerComponent::StartHordeSystem()
{
	if (Waves.Num() == 0) return;
	
	CurrentWaveIndex = 0;
	EnemiesAlive = 0;
	bTimeIsUp = false;
	
	GetWorld()->GetTimerManager().SetTimer(GlobalTimerHandle, this, &UHordeManagerComponent::TimeLimitReached, TotalSurvivalTime, false);
	
	StartWave();
}

void UHordeManagerComponent::StartWave()
{
	if (bTimeIsUp) return;
	
	int32 SafeWaveIndex = FMath::Min(CurrentWaveIndex, Waves.Num() - 1);
	
	FHordeWave CurrentWave = Waves[SafeWaveIndex];
	
	EnemiesSpawnedInCurrentWave = 0;
	EnemiesAlive = 0;

	UE_LOG(LogTemp, Warning, TEXT("Iniciando Oleada %d"), CurrentWaveIndex + 1);
	
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle, 
		this, 
		&UHordeManagerComponent::SpawnEnemy, 
		CurrentWave.SpawnInterval, 
		true, 
		0.0f
	);
}

void UHordeManagerComponent::SpawnEnemy()
{
	int32 SafeWaveIndex = FMath::Min(CurrentWaveIndex, Waves.Num() - 1);
	
	FHordeWave CurrentWave = Waves[SafeWaveIndex];
	
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
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		if (APawn* SpawnedEnemy = GetWorld()->SpawnActor<APawn>(CurrentWave.EnemyClass, SpawnLocation, SpawnRotation, SpawnParams))
		{
			EnemiesAlive++;
			
			SpawnedEnemy->OnDestroyed.AddDynamic(this, &UHordeManagerComponent::OnEnemyDestroyed);
			
			if (AAIController* AIController = Cast<AAIController>(SpawnedEnemy->GetController()))
			{
				APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
				
				UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
				
				if (PlayerPawn && BlackboardComp) BlackboardComp->SetValueAsObject(FName("TargetActor"), PlayerPawn);
			}
			
			EnemiesSpawnedInCurrentWave++;
		}
	}
}

void UHordeManagerComponent::TimeLimitReached()
{
	bTimeIsUp = true;
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	
	CheckWaveState();
}

void UHordeManagerComponent::OnEnemyDestroyed(AActor* DestroyedActor)
{
	EnemiesAlive--;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("EnemyDestroyed. Restantes: %d"), EnemiesAlive));
	CheckWaveState();
}

void UHordeManagerComponent::CheckWaveState()
{
	if (GEngine)
	{
		int32 SafeWaveIndex = FMath::Min(CurrentWaveIndex, Waves.Num() - 1);
		int32 Requeridos = Waves[SafeWaveIndex].EnemiesToSpawn;
        
		FString Msg = FString::Printf(TEXT("CheckWave -> Vivos: %d | Spawneados: %d / %d"), EnemiesAlive, EnemiesSpawnedInCurrentWave, Requeridos);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, Msg);
	}
	
	if (EnemiesAlive <= 0)
	{
		if (bTimeIsUp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Oleada %d terminada"), CurrentWaveIndex + 1);
			OnHordeVictory.Broadcast();
		}
		else
		{
			int32 SafeWaveIndex = FMath::Min(CurrentWaveIndex, Waves.Num() - 1);
			
			if (EnemiesSpawnedInCurrentWave >= Waves[SafeWaveIndex].EnemiesToSpawn)
			{
				CurrentWaveIndex++;
				StartWave();
			}
		}
	} 
}

