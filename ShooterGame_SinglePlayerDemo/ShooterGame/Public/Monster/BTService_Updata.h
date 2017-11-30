// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Updata.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UBTService_Updata : public UBTService
{
	GENERATED_BODY()
public:
	void TickNode(UBehaviorTreeComponent & OwnerComp,uint8 * NodeMemory,float DeltaSeconds);

//	virtual void OnSearchStart(FBehaviorTreeSearchData & SearchData);
protected:
	
	FVector FindLocationInCircle(const FVector& center, float Radius);
};
