// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/AI/Tasks/BTTask_SaveTargetLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SaveTargetLocation::UBTTask_SaveTargetLocation()
{
	NodeName = "Charge to last position Target";
}

EBTNodeResult::Type UBTTask_SaveTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
		
	const TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	const TObjectPtr<APawn> AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;
	
	if (AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName)))
	{
		FVector TargetLocation = TargetActor->GetActorLocation();
		
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
