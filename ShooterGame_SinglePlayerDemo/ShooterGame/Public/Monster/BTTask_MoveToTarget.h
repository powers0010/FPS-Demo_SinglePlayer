// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UBTTask_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, Category = Condition)
	struct  FBlackboardKeySelector TargetKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp,	uint8 * NodeMemory);
	
};
