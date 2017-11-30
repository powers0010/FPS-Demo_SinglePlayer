// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInAttackRange.h"
#include "AIController_Zombine.h"
#include "Monster_Zombie.h"


bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	AAIController_Zombine* ZombineController = Cast<AAIController_Zombine>(OwnerComp.GetAIOwner());
	AMonster_Zombie* Zombine = ZombineController ? Cast<AMonster_Zombie>(ZombineController->GetPawn()) : nullptr;
	if (Zombine)
	{
		return ZombineController->GetIsInAttackRange();
	}
	return true;
}



