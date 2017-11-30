// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToTarget.h"
#include "AIController_Zombine.h"
#include "Monster_Zombie.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"



EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
//	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController_Zombine* ZombineController = Cast<AAIController_Zombine>(OwnerComp.GetOwner());
	AMonster_Zombie* Zombine = ZombineController ? Cast<AMonster_Zombie>(ZombineController->GetPawn())  : nullptr;
	if (!Zombine)
	{
		return EBTNodeResult::Failed;
	}

	AShooterCharacter* Target = ZombineController->GetTarget();
	if (Target)
	{
		ZombineController->MoveToActor(Target,120.f);
	}
	return EBTNodeResult::Failed;
}


