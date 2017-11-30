// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster_Zombie.generated.h"


UENUM(BlueprintType)
enum class EIdleType : uint8
{
	Sitting,
	Eatting,
	Laying,
	Reaching	
};

UENUM(BlueprintType)
enum class EbehaviorType : uint8
{
	Idle,
	IdleToMove,
	MoveAround,
	Chase,
	Attack,
	Died
};


class AShooterCharacter;
class UAnimMontage;
class UBehaviorTree;
class UPawnSensingComponent;
class UBoxComponent;


UCLASS()
class SHOOTERGAME_API AMonster_Zombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster_Zombie();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PlayHitAnim(AActor* DamageCauser);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable , Category = "Attack")
	void InitHandBox(UBoxComponent* left , UBoxComponent* right);

	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "PawnSensing"))
		UPawnSensingComponent* PawnSensingCom;

	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "Attack"))
		UBoxComponent* AttackRange;

	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "Attack"))
		UBoxComponent* LefthandBox;

	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "Attack"))
		UBoxComponent* RighthandBox;

	UFUNCTION()
		void OnSeeingPawn(APawn* Pawn/*, const FVector& Location, float Volume*/);

	UFUNCTION()
		void OnBeginOverlapForAttackRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnEndOverLapForAttackRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnBeginOverlapForHandleBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


	UPROPERTY(EditDefaultsOnly , category = Health)
		float Health;

	UPROPERTY(EditDefaultsOnly, category = Anim)
		TArray<UAnimMontage*> AttackAnim;

	UPROPERTY(EditDefaultsOnly, category = Anim)
		TArray<UAnimationAsset*> DieAnim;

	UPROPERTY(EditDefaultsOnly, category = Anim)
		UAnimMontage* HitAnim_Right;
	UPROPERTY(EditDefaultsOnly, category = Anim)
		UAnimMontage* HitAnim_Left;
	UPROPERTY(EditDefaultsOnly, category = Anim)
		UAnimMontage* HitAnim_Font;
	UPROPERTY(EditDefaultsOnly, category = Anim)
		UAnimMontage* HitAnim_Back;

	UPROPERTY(EditDefaultsOnly, category = Anim)
		UAnimMontage* SittingToIdle;
	UPROPERTY(EditDefaultsOnly, category = Anim)
		UAnimMontage* EattingToIdle;
	UPROPERTY(EditDefaultsOnly, category = Anim)
		UAnimMontage* LayingToIdle;
	UPROPERTY(EditDefaultsOnly, category = Anim)
		UAnimMontage* ReachingToIdle;

	//播放BeHit动画 的伤害 临界值
	UPROPERTY(EditDefaultsOnly, category = Anim)
		float DamageEnoughToReact;

	bool bisalive;

	bool bIsAttacking;

	UPROPERTY(EditDefaultsOnly, category = "Attack")
	float DamagePoint;

	void DealDamage(APawn* targetpawn);

	UPROPERTY(EditAnywhere, Category = Behavior)
		UBehaviorTree* ZombieBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Behavior)
		EIdleType IdleType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		EbehaviorType BehaviorType;

	UPROPERTY(EditAnywhere, Category = Behavior)
		float MoveRadiu;

	FTimerHandle Timer_BehaviorIdleEnd;

	void OnBehaviorIdleEnd();
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
// 		AShooterCharacter* TargetPawn;

	float PlayAnimFromIdleToStand();

public:	
	void OnDeath();

	bool GetIsAlive();

	UBehaviorTree* GetBehaviorTree();
	
	EIdleType GetIdleType() const;

	EbehaviorType GetBehaviorType() const;

	float GetMoveRadiu();

	UFUNCTION(BlueprintCallable, category = "Behavior")
	void SetBehaviorType(EbehaviorType newtype);

	float PlayAttackAnim();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetIsAttacking(bool newbool);

/*	AShooterCharacter* GetTargetPawn() const;*/

/*	void SetTargetPawn(AShooterCharacter* Target);*/

};
