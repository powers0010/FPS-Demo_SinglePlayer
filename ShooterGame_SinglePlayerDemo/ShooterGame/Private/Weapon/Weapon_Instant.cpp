// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_Instant.h"
#include "ShooterCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "ShooterImpactEffect.h"



void AWeapon_Instant::FireWeapon()
{
	//计算子弹发射方向
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);

	const float CurrentSpread = InstantWeaponConfig.WeaponSpreed;
	const float CurrentHalfAngle = FMath::DegreesToRadians(CurrentSpread / 2.f);
	const FVector AimDir = GetAdjustAim();
	
//	UE_LOG(LogTemp, Warning, TEXT("Rotation: %s "), *AimDir.ToString());
//	const FVector ShootDir = WeaponRandomStream.VRandCone(AimDie,0,0/*CurrentHalfAngle, CurrentHalfAngle*/);

	//伤害对象
	const FVector StartLocation = GetMuzzleLocation();
	const float DamageRange = InstantWeaponConfig.WeaponRange;

	const FHitResult HitResult = WeaponTrace(StartLocation, StartLocation + DamageRange*AimDir);

	//伤害处理
	ProcessInstantHit(HitResult, StartLocation, AimDir, RandomSeed, CurrentSpread);
//	UE_LOG(LogTemp, Warning, TEXT("Fire Weapon"));
}


void AWeapon_Instant::ProcessInstantHit(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir, int32 RandomSeed, float ReticleSpreed)
{
	ProcessInstantHit_Confirm(Impact, Orign, ShootDir, RandomSeed, ReticleSpreed);
}

void AWeapon_Instant::ProcessInstantHit_Confirm(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir, int32 RandomSeed, float ReticleSpreed)
{
	//生命值处理
	DealDamage(Impact , ShootDir);

	//生成开枪特效
	const FVector EndTrace = Orign + ShootDir*InstantWeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
//	UE_LOG(LogTemp, Warning, TEXT("EndPoint : %s"), *EndPoint.ToString());
	SpawnTraceEffect(EndPoint);

	//生成击中特效
	SpawnImpactEffects(Impact);
}

void AWeapon_Instant::DealDamage(const FHitResult& Impact, const FVector& ShootDir)
{
	FPointDamageEvent PointDamage;
	PointDamage.DamageTypeClass = InstantWeaponConfig.DamageType;
	PointDamage.HitInfo = Impact;
	PointDamage.ShotDirection = ShootDir;
	PointDamage.Damage = InstantWeaponConfig.damage;

	if (Impact.GetActor())
	{
		Impact.GetActor()->TakeDamage(PointDamage.Damage, PointDamage, PawnOwner->GetController(), this );
	}
}

void AWeapon_Instant::SpawnTraceEffect(const FVector& EndPoint)
{
	if (FireEffect)
	{
		const FVector Orign = GetMuzzleLocation();
		UParticleSystemComponent* TralEmitter = UGameplayStatics::SpawnEmitterAtLocation(this, FireEffect, Orign);
		if (TralEmitter)
		{
			TralEmitter->SetVectorParameter(TrailTargetParam , EndPoint);

//			UE_LOG(LogTemp, Warning, TEXT("TralEmitter"));
		}
	}
}

void AWeapon_Instant::SpawnImpactEffects(const FHitResult& Impact)
{
	if (ImpactEffectActor && Impact.bBlockingHit)
	{
		const FTransform SpawnTrans(Impact.Normal.Rotation(), Impact.ImpactPoint);
		AShooterImpactEffect* ImpactEffect = GetWorld()->SpawnActorDeferred<AShooterImpactEffect>(ImpactEffectActor, SpawnTrans);
		if (ImpactEffect)
		{
			ImpactEffect->SetHitResult(Impact);
			UGameplayStatics::FinishSpawningActor(ImpactEffect, SpawnTrans);
//			UE_LOG(LogTemp, Warning, TEXT("FinishSpawningActor ImpactEffect"));
		}
	}
}