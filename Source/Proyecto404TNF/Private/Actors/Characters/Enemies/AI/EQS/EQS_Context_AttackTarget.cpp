// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/AI/EQS/EQS_Context_AttackTarget.h"

#include "Actors/Characters/Enemies/AI/EnemyAIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEQS_Context_AttackTarget::ProvideContext(FEnvQueryInstance& QueryInstance,
                                               FEnvQueryContextData& ContextData) const
{
	AActor* QuerierActor = Cast<AActor>(QueryInstance.Owner.Get());
	if (!QuerierActor) return;
	
	AEnemyAIController* AIController = nullptr;
	
	AIController = Cast<AEnemyAIController>(QuerierActor);
	if (!AIController)
	{
		if (APawn* QuerierPawn = Cast<APawn>(QuerierActor))
		{
			AIController = Cast<AEnemyAIController>(QuerierPawn->GetController());
		}
	}
	
	if (AIController){
		AActor* TargetActor = AIController->AttackTarget;
	
		if (TargetActor)
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetActor);
		}
	}
}
