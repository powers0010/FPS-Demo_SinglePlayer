// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterZombine_Anim.h"


void UMonsterZombine_Anim::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!Owner)
	{
		Owner = Cast<AMonster_Zombie>(TryGetPawnOwner());
	}
	if (Owner)
	{
		Speed = Owner->GetVelocity().Size();
		const FRotator WorldToRoot = Owner->ActorToWorld().GetRotation().Rotator().GetInverse();
		const FVector VelocityInRoot = WorldToRoot.RotateVector(Owner->GetVelocity());
		SpeedInForward = VelocityInRoot.X;
		SpeedInRight = VelocityInRoot.Y;

		IdleType = Owner->GetIdleType();
		BehaviorType1 = Owner->GetBehaviorType();
	}
}

