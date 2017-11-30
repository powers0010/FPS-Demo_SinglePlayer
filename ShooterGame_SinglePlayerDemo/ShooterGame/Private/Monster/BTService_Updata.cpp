// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Updata.h"
#include "AIController_Zombine.h"
#include "Monster_Zombie.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"



void UBTService_Updata::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	AAIController_Zombine* ZombineController = Cast<AAIController_Zombine>(OwnerComp.GetAIOwner());
//	UE_LOG(LogTemp, Warning, TEXT("I am Receive Tick"));

	AMonster_Zombie* Zombine = ZombineController ? Cast<AMonster_Zombie>(ZombineController->GetPawn()) : nullptr;

	if (Zombine && EbehaviorType::MoveAround == Zombine->GetBehaviorType())
	{
		FVector TargetLocation = ZombineController->GetTargetLocationInBT();
		if (Zombine->GetActorLocation().Equals(TargetLocation, 100.f) || TargetLocation == FVector(ForceInitToZero))
		{
			TargetLocation = FindLocationInCircle(Zombine->GetActorLocation(), Zombine->GetMoveRadiu());
			ZombineController->SetTargetLocation(TargetLocation);
		}

//		UE_LOG(LogTemp, Warning, TEXT("GetActorLocation %s"), *Zombine->GetActorLocation().ToString());
//		UE_LOG(LogTemp, Warning, TEXT("TargetLocation %s"), *TargetLocation.ToString());
	}
}

// void  UBTService_Updata::OnSearchStart(FBehaviorTreeSearchData & SearchData)
// {
// 	Super::OnSearchStart(SearchData);
// 	AAIController_Zombine* ZombineController = Cast<AAIController_Zombine>(SearchData.OwnerComp.GetAIOwner());
// 	UE_LOG(LogTemp, Warning, TEXT("OnSearchStart"));
// 
// 	AMonster_Zombie* Zombine = ZombineController ? Cast<AMonster_Zombie>(ZombineController->GetPawn()) : nullptr;
// 
// 	if (Zombine && EbehaviorType::MoveAround == Zombine->GetBehaviorType())
// 	{
// 		FVector TargetLocation = FindLocationInCircle(Zombine->GetActorLocation(), 2000.f);
// 		ZombineController->SetTargetLocation(TargetLocation);
// 	}
// }


FVector UBTService_Updata::FindLocationInCircle(const FVector& center, float Radius)
{
	//	UE_LOG(LogTemp, Warning, TEXT("MoveInCircle"));
	FVector TargetLocation = UNavigationSystem::GetRandomReachablePointInRadius(this, center, Radius);

	return TargetLocation;
}