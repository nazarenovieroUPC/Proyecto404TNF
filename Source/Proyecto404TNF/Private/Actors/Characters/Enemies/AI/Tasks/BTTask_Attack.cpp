// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/AI/Tasks/BTTask_Attack.h"

#include "AIController.h"
#include "Interfaces/EnemyInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack to Player";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	
	TObjectPtr<APawn> AIPawn = AIController->GetPawn();
	
	if (!AIPawn) return EBTNodeResult::Failed;
	
	if (AIPawn->Implements<UEnemyInterface>())
	{
		IEnemyInterface::Execute_EnemyAttack(AIPawn);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
