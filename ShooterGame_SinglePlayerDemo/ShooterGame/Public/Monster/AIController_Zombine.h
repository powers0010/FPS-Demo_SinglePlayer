// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Zombine.generated.h"

/**
 * 
 */
class UBehaviorTreeComponent;
class UBlackboardComponent;
class AShooterCharacter;

UCLASS()
class SHOOTERGAME_API AAIController_Zombine : public AAIController
{
	GENERATED_BODY()
public:
	AAIController_Zombine();

	void SetTarget(APawn* TargetPawn);

	AShooterCharacter* GetTarget() const;

	void SetTargetLocation(FVector TargetLocation);

	FVector GetTargetLocationInBT();

	void SetIsInAttackRange(bool Newboll);

	UFUNCTION(BlueprintCallable, category = "Attack")
	bool GetIsInAttackRange();

private:
	UPROPERTY(transient)
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
	UBehaviorTreeComponent* BehaviorComp;
	
	int32 TargetKeyID;
	int32 TargetLocationKeyID;
	int32 IsInAttackRangeKeyID;

protected:
	virtual void Possess(APawn* PossessedPawn) override;

	virtual void UnPossess() override;

};
