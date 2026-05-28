// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/AI/Tasks/BTTask_Patrol.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = "Find Random Poss to Patrol";
	
	PatrolRadius = 1500.f;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	//if (!AIController) return EBTNodeResult::Failed;
	
	TObjectPtr<APawn> AIPawn = AIController->GetPawn();
	
	TObjectPtr<UNavigationSystemV1> NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	
	if (!NavSystem || !AIPawn || !AIController) return EBTNodeResult::Failed;
	
	FVector Origin = AIPawn->GetActorLocation();
	
	FNavLocation RandLocation;
	
	if (NavSystem->GetRandomReachablePointInRadius(Origin, PatrolRadius, RandLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, RandLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

