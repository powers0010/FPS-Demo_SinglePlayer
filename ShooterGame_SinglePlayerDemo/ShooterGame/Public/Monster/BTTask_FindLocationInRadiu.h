// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_FindLocationInRadiu.generated.h"

/**
 * 
 */
class UBehaviorTreeComponent;


UCLASS()
class SHOOTERGAME_API UBTTask_FindLocationInRadiu : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory);
	
	FVector FindLocationInCircle(const FVector& center, float Radius);
};
