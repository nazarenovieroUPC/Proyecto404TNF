// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/AI/Decorators/BTDecorator_TargetDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_TargetDistance::UBTDecorator_TargetDistance()
{
	NodeName = "TargetDistance";
	
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_TargetDistance, BlackboardKey), AActor::StaticClass());
}

bool UBTDecorator_TargetDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;
	
	TObjectPtr<APawn> AIPawn = AIController->GetPawn();
	
	if (!AIPawn) return false;
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;
	
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName));
	if (!TargetActor) return false;
	
	return AIPawn->GetDistanceTo(TargetActor) <= TargetDistance;
	
	
	
	
}
