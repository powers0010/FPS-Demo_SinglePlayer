// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster_Zombie.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/AIModule/Classes/Perception/PawnSensingComponent.h"
#include "ShooterCharacter.h"
#include "AIController_Zombine.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"

// Sets default values
AMonster_Zombie::AMonster_Zombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingCom = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingCom"));
	
	AttackRange = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackRange"));
	AttackRange->SetupAttachment(RootComponent);

	bisalive = true;
	DamageEnoughToReact = 40.f;
	IdleType = EIdleType::Sitting;
	BehaviorType = EbehaviorType::Idle;
	MoveRadiu = 1500.f;
	bIsAttacking = false;
	DamagePoint = 10.f;
}

// Called when the game starts or when spawned
void AMonster_Zombie::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensingCom)
	{
		PawnSensingCom->OnSeePawn.AddDynamic(this, &AMonster_Zombie::OnSeeingPawn);
	}
	if (AttackRange)
	{
		AttackRange->OnComponentBeginOverlap.AddDynamic(this, &AMonster_Zombie::OnBeginOverlapForAttackRange);
		AttackRange->OnComponentEndOverlap.AddDynamic(this, &AMonster_Zombie::OnEndOverLapForAttackRange);
	}

}

void AMonster_Zombie::InitHandBox(UBoxComponent* left ,UBoxComponent* right)
{
	if (left)
	{
		LefthandBox = left;
	}
	if (right)
	{
		RighthandBox = right;
	}
	if (LefthandBox && RighthandBox)
	{
		LefthandBox->OnComponentBeginOverlap.AddDynamic(this, &AMonster_Zombie::OnBeginOverlapForHandleBox);
		RighthandBox->OnComponentBeginOverlap.AddDynamic(this, &AMonster_Zombie::OnBeginOverlapForHandleBox);
	}
}

float AMonster_Zombie::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = 0.f;
	if (bisalive)
	{
		//保存 经过父类计算的实际伤害
		ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		if (ActualDamage > 0.f)
		{
			if (BehaviorType<EbehaviorType::MoveAround)
			{
				BehaviorType = EbehaviorType::IdleToMove;
			}
			else if(BehaviorType == EbehaviorType::MoveAround)
			{
				AShooterCharacter* targetpawn = Cast<AShooterCharacter>(EventInstigator->GetPawn());
				if (targetpawn)
				{
					OnSeeingPawn(targetpawn);
				}
			}
			Health -= ActualDamage;
			
			if (Health<=0.f)
			{
				Health = 0.f;
				OnDeath();
			}
			else
			{
				if (ActualDamage>DamageEnoughToReact)
				{
					PlayHitAnim(DamageCauser);
					if (BehaviorType == EbehaviorType::IdleToMove)
					{
						BehaviorType = EbehaviorType::MoveAround;
					}			
				}
			}
//			UE_LOG(LogTemp, Warning, TEXT("Zombie Health: %f"), Health);
		}
	}

	return ActualDamage;
}

void AMonster_Zombie::OnSeeingPawn(APawn* Pawn/*, const FVector& Location, float Volume*/)
{
	AShooterCharacter* tagetpawn = Cast<AShooterCharacter>(Pawn);
	AAIController_Zombine* controller = Cast<AAIController_Zombine>(GetController());
	if (controller && tagetpawn && BehaviorType> EbehaviorType::IdleToMove)
	{
		controller->SetTarget(tagetpawn);
	}
// 	if (BehaviorType<EbehaviorType::Chase)
// 	{
// 		BehaviorType = EbehaviorType::Chase;
// 	}
}

void AMonster_Zombie::OnDeath()
{
//	UE_LOG(LogTemp, Warning, TEXT("I am Die"));
	if (bisalive && GetMesh() && DieAnim.Num()>0)
	{
		int32 Index = FMath::RandRange(0, DieAnim.Num() - 1);
		GetMesh()->PlayAnimation (DieAnim[Index],false);
	}
	DetachFromControllerPendingDestroy();
	bisalive = false;
}

void AMonster_Zombie::PlayHitAnim(AActor* DamageCauser)
{
	UAnimMontage* AnimToPlay = nullptr;

	if (DamageCauser && nullptr == GetCurrentMontage())
	{
		const FVector CauserLocation = DamageCauser->GetActorLocation();
		const FVector FaceDir = CauserLocation - GetActorLocation();
		const FRotator FaceRot = FaceDir.Rotation();
		const FRotator RelativeRot = FaceRot - GetActorRotation();
		float Yaw = FRotator::NormalizeAxis(RelativeRot.Yaw);

		if (Yaw <45.f && Yaw>-45.f)
		{
			AnimToPlay = HitAnim_Font;
		}
		else if(Yaw >45.f &&Yaw <135.f)
		{
			AnimToPlay = HitAnim_Right;
		}
		else if (Yaw > -135.f && Yaw < -45.f)
		{
			AnimToPlay = HitAnim_Left;
		}
		else if (Yaw > 135.f || Yaw < -135.f)
		{
			AnimToPlay = HitAnim_Back;
		}
	}
	if (AnimToPlay)
	{
		PlayAnimMontage(AnimToPlay);
//		UE_LOG(LogTemp, Warning, TEXT("PlayAnimMontage"));
	}
}

bool AMonster_Zombie::GetIsAlive()
{
	return bisalive;
}

UBehaviorTree*  AMonster_Zombie::GetBehaviorTree()
{
	return ZombieBehavior;
}

EIdleType AMonster_Zombie::GetIdleType() const
{
	return IdleType;
}

EbehaviorType AMonster_Zombie::GetBehaviorType() const
{
	return BehaviorType;
}

float AMonster_Zombie::PlayAnimFromIdleToStand()
{
	float AnimDuring = 0.5f;

	
//	UE_LOG(LogTemp, Warning, TEXT("MoveAround"));

	UAnimMontage* Animation = nullptr;

	switch (IdleType)
	{
	case EIdleType::Sitting:
		Animation = SittingToIdle;
		break;
	case EIdleType::Eatting:
		Animation = EattingToIdle;
		break;
	case EIdleType::Laying:
		Animation = LayingToIdle;
		break;
	case EIdleType::Reaching:
		Animation = ReachingToIdle;
		break;
	default:
		break;
	}
	if (Animation)
	{
		AnimDuring = GetMesh()->AnimScriptInstance->Montage_Play(Animation);
	}
	return AnimDuring;
}

float AMonster_Zombie::PlayAttackAnim()
{
	UAnimMontage* Animation = nullptr;
	if (bisalive && GetCurrentMontage()==nullptr && AttackAnim.Num()>0)
	{
		Animation = AttackAnim[FMath::RandRange(0, AttackAnim.Num())];
	}
	return Animation ? GetMesh()->AnimScriptInstance->Montage_Play(Animation) : 0.5f;
}

// AShooterCharacter* AMonster_Zombie::GetTargetPawn() const
// {
// 	return TargetPawn;
// }
// 
// void AMonster_Zombie::SetTargetPawn(AShooterCharacter* Target)
// {
// 	if (Target)
// 	{
// 		TargetPawn = Target;
// 	}
// }
// 	

float AMonster_Zombie::GetMoveRadiu()
{
	return MoveRadiu;
}

void AMonster_Zombie::OnBehaviorIdleEnd()
{
	BehaviorType = EbehaviorType::MoveAround;
	GetWorld()->GetTimerManager().ClearTimer(Timer_BehaviorIdleEnd);
}

void AMonster_Zombie::SetBehaviorType(EbehaviorType newtype)
{
	BehaviorType = newtype;
}

void AMonster_Zombie::OnBeginOverlapForAttackRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AShooterCharacter* target = Cast<AShooterCharacter>(OtherActor);
	AAIController_Zombine* controller = Cast<AAIController_Zombine>(GetController());
	if (controller && target)
	{
		controller->SetIsInAttackRange(true);
	}
}


void AMonster_Zombie::OnEndOverLapForAttackRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AShooterCharacter* target = Cast<AShooterCharacter>(OtherActor);
	AAIController_Zombine* controller = Cast<AAIController_Zombine>(GetController());
	if (controller && target)
	{
		controller->SetIsInAttackRange(false);
	}
}

void AMonster_Zombie::OnBeginOverlapForHandleBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AShooterCharacter* target = Cast<AShooterCharacter>(OtherActor);
	AAIController_Zombine* controller = Cast<AAIController_Zombine>(GetController());
	if (controller && target)
	{
		if (bisalive && bIsAttacking)
		{
			DealDamage(target);
		}
	}
//	UE_LOG(LogTemp, Warning, TEXT("OnBeginOverlapForHandleBox"));
}

void AMonster_Zombie::DealDamage(APawn* targetpawn)
{
	if (targetpawn)
	{
		FPointDamageEvent PointDamage;
		PointDamage.Damage = DamagePoint;
		targetpawn->TakeDamage(PointDamage.Damage, PointDamage, GetController(), this);
//		UE_LOG(LogTemp, Warning, TEXT("I Hit Player"));
	}
}

void AMonster_Zombie::SetIsAttacking(bool newbool)
{
	bIsAttacking = newbool;
}