// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/AI/EnemyAIController.h"

#include "Actors/Characters/Enemies/EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Proyecto404TNF/Proyecto404TNFCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	
	SightConfig->SightRadius = 700.f;
	SightConfig->LoseSightRadius = 1000.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->PointOfViewBackwardOffset = 200.f;
	SightConfig->NearClippingRadius = 100.f;
	SightConfig->SetMaxAge(0.2f);
	
	DamageConfig->SetMaxAge(5.f);

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	
	PerceptionComponent->ConfigureSense(*DamageConfig);
	
	SetPerceptionComponent(*PerceptionComponent);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (PerceptionComponent) PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	AEnemyBase* Enemy = Cast<AEnemyBase>(InPawn);
	if (Enemy != nullptr)
	{
		if (Enemy->EnemyBehaviorTree != nullptr) {RunBehaviorTree(Enemy->EnemyBehaviorTree);}
	}
	
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor != nullptr && Actor->IsA(AProyecto404TNFCharacter::StaticClass())){
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), Actor);
			GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), Actor->GetActorLocation());
			
			AttackTarget = Actor;
		}
		else
		{
			if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
			{
				
				GetBlackboardComponent()->ClearValue(FName("TargetActor"));
				GetBlackboardComponent()->SetValueAsVector(FName("LastLocation"), Stimulus.StimulusLocation);
			}
		}
	}
}
