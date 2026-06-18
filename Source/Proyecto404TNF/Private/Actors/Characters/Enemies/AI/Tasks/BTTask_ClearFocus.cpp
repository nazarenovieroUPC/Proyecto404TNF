// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/AI/Tasks/BTTask_ClearFocus.h"

#include "AIController.h"

UBTTask_ClearFocus::UBTTask_ClearFocus()
{
	NodeName = "Clear Focus to Player";
}

EBTNodeResult::Type UBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		
		return EBTNodeResult::Succeeded;
	}
	
	
	return EBTNodeResult::Failed;
}
