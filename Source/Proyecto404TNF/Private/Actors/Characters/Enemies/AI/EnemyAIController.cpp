// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/AI/EnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Proyecto404TNF/Proyecto404TNFCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	
	SightConfig->SightRadius = 700.f;
	SightConfig->LoseSightRadius = 900.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->PointOfViewBackwardOffset = 200.f;
	SightConfig->NearClippingRadius = 300.f;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (BehaviorTree != nullptr){
		
		RunBehaviorTree(BehaviorTree);
		
		if (GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, "Se cargo el BT");}
	}
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor != nullptr && Actor->IsA(AProyecto404TNFCharacter::StaticClass())){
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), Actor);
			GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), Actor->GetActorLocation());
			
		}
		else
		{
			GetBlackboardComponent()->ClearValue(FName("TargetActor"));
			
			GetBlackboardComponent()->SetValueAsVector(FName("LastLocation"), Stimulus.StimulusLocation);
		}
	}
}
