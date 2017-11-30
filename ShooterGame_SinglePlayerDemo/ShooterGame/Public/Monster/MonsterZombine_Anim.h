// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Monster_Zombie.h"
#include "MonsterZombine_Anim.generated.h"

/**
 * 
 */

UCLASS()
class SHOOTERGAME_API UMonsterZombine_Anim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
		AMonster_Zombie* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	EIdleType IdleType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	EbehaviorType BehaviorType1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	float SpeedInForward;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	float SpeedInRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	float Speed;
};
