// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_Attack.h"
#include "AIController_Zombine.h"
#include "Monster_Zombie.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AAIController_Zombine* ZombineController = Cast<AAIController_Zombine>(OwnerComp.GetOwner());
	AMonster_Zombie* Zombine = ZombineController ? Cast<AMonster_Zombie>(ZombineController->GetPawn()) : nullptr;

	if (Zombine)
	{
		Zombine->PlayAttackAnim();
	}
	return EBTNodeResult::Type::Failed;
}
