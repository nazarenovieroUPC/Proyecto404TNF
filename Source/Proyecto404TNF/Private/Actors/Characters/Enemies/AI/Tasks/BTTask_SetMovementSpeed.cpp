// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Characters/Enemies/AI/Tasks/BTTask_SetMovementSpeed.h"
#include "AIController.h"

#include "Interfaces/EnemyInterface.h"

UBTTask_SetMovementSpeed::UBTTask_SetMovementSpeed()
{
	NodeName = "Set Movement Speed for different actions";
}

EBTNodeResult::Type UBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	
	TObjectPtr<APawn> AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;
	
	if (AIPawn->Implements<UEnemyInterface>())
	{
		IEnemyInterface::Execute_SetMovementSpeed(AIPawn, MovementSpeed);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
