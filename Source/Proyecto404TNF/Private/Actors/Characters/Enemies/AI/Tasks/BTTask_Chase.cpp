// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/AI/Tasks/BTTask_Chase.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Chase::UBTTask_Chase()
{
	
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	
	TObjectPtr<APawn> AIPawn = AIController->GetPawn();
	
	TObjectPtr<UNavigationSystemV1> NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	
	if (!NavSystem || !AIPawn || !AIController) return EBTNodeResult::Failed;
	
	return EBTNodeResult::Failed;
}
