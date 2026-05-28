// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "EnemyAIController.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;

UCLASS()
class PROYECTO404TNF_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyAIController();
	
	UPROPERTY(Blueprintable, EditAnywhere, BlueprintReadWrite, Category = "Behavior Tree")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(Blueprintable, EditAnywhere, BlueprintReadWrite, Category = "Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
};
