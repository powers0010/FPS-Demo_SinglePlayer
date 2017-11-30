// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController_Zombine.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster_Zombie.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardData.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


AAIController_Zombine::AAIController_Zombine()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));

	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void AAIController_Zombine::Possess(APawn* PossessedPawn)
{
	Super::Possess(PossessedPawn);

	AMonster_Zombie* Zombie = Cast<AMonster_Zombie>(PossessedPawn);

	if (Zombie && Zombie->GetBehaviorTree())
	{
		if (Zombie->GetBehaviorTree()->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Zombie->GetBehaviorTree()->BlackboardAsset);
		}
		TargetKeyID = BlackboardComp->GetKeyID("Target");
		TargetLocationKeyID = BlackboardComp->GetKeyID("TargetLocation");
		IsInAttackRangeKeyID = BlackboardComp->GetKeyID("IsInAttackRange");

		BehaviorComp->StartTree(*Zombie->GetBehaviorTree());
	}
}

void AAIController_Zombine::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

void AAIController_Zombine::SetTarget(APawn* TargetPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(TargetKeyID,TargetPawn);
	}
}

AShooterCharacter* AAIController_Zombine::GetTarget() const
{
	if (BlackboardComp)
	{
		return Cast<AShooterCharacter>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(TargetKeyID));
	}
	return nullptr;
}

void AAIController_Zombine::SetTargetLocation(FVector TargetLocation)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Vector>(TargetLocationKeyID, TargetLocation);
	}
}

FVector AAIController_Zombine::GetTargetLocationInBT()
{
	FVector location = FVector(ForceInitToZero);
	if (BlackboardComp && BlackboardComp->IsVectorValueSet(TargetLocationKeyID))
	{
		location = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(TargetLocationKeyID);
	}

	return location;
}

void AAIController_Zombine::SetIsInAttackRange(bool Newboll)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Bool>(IsInAttackRangeKeyID, Newboll);
	}
}

bool AAIController_Zombine::GetIsInAttackRange()
{
	if (BlackboardComp)
	{
		return BlackboardComp->GetValue<UBlackboardKeyType_Bool>(IsInAttackRangeKeyID);
	}
	return false;
}
