// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_machine.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "ShooterImpactEffect.h"
#include "ShooterCharacter.h"


AWeapon_machine::AWeapon_machine()
{
	FireAudioCom = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioCom"));
	FireAudioCom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FireAudioCom->bAutoActivate = false;

	FireParticleCom = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticleCom"));
	FireParticleCom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FireParticleCom->bAutoActivate = false;

	WeaponRange = 10000.f;
}

void AWeapon_machine::SimulateWeaponFire()
{
	if (FireAudioCom)
	{
		FireAudioCom->SetActive(true);
	}
	if (FireParticleCom)
	{
		FireParticleCom->SetActive(true);
	}

}

void AWeapon_machine::OnStartFire()
{
	SimulateWeaponFire();
	FireWeapon();
	GetWorld()->GetTimerManager().SetTimer(Timer_OnStartFire, this, &AWeapon_machine::FireWeapon, WeaponConfig.FireCD, true);
}

void AWeapon_machine::OnStopFire()
{
	if (FireAudioCom)
	{
		FireAudioCom->SetActive(false);
	}
	if (FireParticleCom)
	{
		FireParticleCom->SetActive(false);
	}
	GetWorld()->GetTimerManager().ClearTimer(Timer_OnStartFire);
}

void AWeapon_machine::FireWeapon()
{
	if (AmmoCount > 0)
	{
		FHitResult HitResult(ForceInit);
		FindHitResult(HitResult);
		const FVector AimDie = GetAdjustAim();
		ProcessSniperHit(HitResult, GetMuzzleLocation(), AimDie);

		AmmoCount--;
	}
}

void AWeapon_machine::FindHitResult(FHitResult& HitResult)
{
	const FVector StartLocation = GetMuzzleLocation();
	const FVector ArmDir = GetAdjustAim();
	HitResult = WeaponTrace(StartLocation, StartLocation + ArmDir * WeaponRange);

}

void AWeapon_machine::SpawnImpactEffects(const FHitResult& Impact)
{
	if (ImpactEffectActor && Impact.bBlockingHit)
	{
		const FTransform HitTrans = FTransform(Impact.Normal.Rotation(), Impact.ImpactPoint);
		AShooterImpactEffect* ImpactEffect = GetWorld()->SpawnActorDeferred<AShooterImpactEffect>(ImpactEffectActor, HitTrans);
		if (ImpactEffect)
		{
			ImpactEffect->SetHitResult(Impact);
			ImpactEffect->FinishSpawning(HitTrans);
		}
	}
}

void AWeapon_machine::ProcessSniperHit(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir)
{
	ProcessSniprHit_Confirm(Impact, Orign, ShootDir);
}

void AWeapon_machine::ProcessSniprHit_Confirm(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir)
{
	//生命值处理
	DealDamage(Impact, ShootDir);

	//生成开枪特效
	const FVector EndTrace = Orign + ShootDir*SniperWeaponData.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
	//	UE_LOG(LogTemp, Warning, TEXT("EndPoint : %s"), *EndPoint.ToString());

	//生成击中特效
	SpawnImpactEffects(Impact);
}

void AWeapon_machine::DealDamage(const FHitResult& Impact, const FVector& ShootDir)
{
	FPointDamageEvent PointDamage;
	PointDamage.DamageTypeClass = SniperWeaponData.DamageType;
	PointDamage.HitInfo = Impact;
	PointDamage.ShotDirection = ShootDir;
	PointDamage.Damage = SniperWeaponData.damage;

	if (Impact.GetActor())
	{
		Impact.GetActor()->TakeDamage(PointDamage.Damage, PointDamage, PawnOwner->GetController(), this);
	}
}

void AWeapon_machine::OnReload()
{
	Super::OnReload();
	OnStopFire();
}

void AWeapon_machine::OnEquip()
{
	Super::OnEquip();
	OnStopFire();
}