// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Characters/Enemies/AI/Tasks/BTTask_FocusTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FocusTarget::UBTTask_FocusTarget()
{
	NodeName = "Focus To Target";
}

EBTNodeResult::Type UBTTask_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	const TObjectPtr<APawn> AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	if (AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName)))
	{
		AIController->SetFocus(TargetActor, EAIFocusPriority::Gameplay);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
