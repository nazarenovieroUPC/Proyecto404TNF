// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetMovementSpeed.generated.h"

enum class EMovementSpeed : uint8;
/**
 * 
 */
UCLASS()
class PROYECTO404TNF_API UBTTask_SetMovementSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_SetMovementSpeed();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementSpeed MovementSpeed;
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
