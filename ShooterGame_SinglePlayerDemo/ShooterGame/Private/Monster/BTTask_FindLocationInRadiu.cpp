// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindLocationInRadiu.h"
#include "AIController_Zombine.h"
#include "Monster_Zombie.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"

EBTNodeResult::Type UBTTask_FindLocationInRadiu::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory)
{
//	UE_LOG(LogTemp, Warning, TEXT("ExecuteTask"));
	AAIController_Zombine* ZombineController = Cast<AAIController_Zombine>(OwnerComp.GetOwner());
	AMonster_Zombie* Zombine = ZombineController ? Cast<AMonster_Zombie>(ZombineController->GetPawn()) : nullptr;

	if (Zombine && EbehaviorType::MoveAround == Zombine->GetBehaviorType())
	{
		FVector TargetLocation = FindLocationInCircle(Zombine->GetActorLocation(), Zombine->GetMoveRadiu());
		ZombineController->SetTargetLocation(TargetLocation);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

FVector UBTTask_FindLocationInRadiu::FindLocationInCircle(const FVector& center , float Radius)
{
//	UE_LOG(LogTemp, Warning, TEXT("MoveInCircle"));
	FVector TargetLocation = UNavigationSystem::GetRandomReachablePointInRadius(this, center, Radius);

	return TargetLocation;
}