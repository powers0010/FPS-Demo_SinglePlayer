// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_sniper.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "ShooterCharacter.h"
#include "SniperTargetWidget.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ShooterPlayerController.h"
#include "ShooterImpactEffect.h"
#include "ShooterGame.h"



AWeapon_sniper::AWeapon_sniper():TargetFOV(20.f), NormalFov(90.f)
{
	SniperViewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SniperView"));
	SniperViewMesh->SetupAttachment(RootComponent);
	SniperViewMesh->CastShadow = false;
}

void AWeapon_sniper::SimulateWeaponFire()
{
	Super::SimulateWeaponFire();
	//开火粒子
	if (FireEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(FireEffect, Mesh1P, MuzzleAttachPoint ,PawnOwner->GetActorForwardVector());
	}
}

void AWeapon_sniper::FireWeapon()
{
	FHitResult HitResult(ForceInit);
	FindHitResult(HitResult);		// 同时处理ProcessSniperHit

}

void AWeapon_sniper::OnStartFire()
{
	if (AmmoCount > 0/* && PawnOwner->GetIsTargeting()*/)
	{
		SimulateWeaponFire();
		FireWeapon();
		AmmoCount--;
//		UE_LOG(LogTemp, Warning, TEXT("Fire!"));
	}
}

void AWeapon_sniper::OnStopFire()
{
	OnStopTarget();
}

void AWeapon_sniper::FindHitResult(FHitResult& HitResult)
{
	AShooterPlayerController* PlayerController = PawnOwner->Instigator ? Cast<AShooterPlayerController>(PawnOwner->Instigator->GetController()) : nullptr;
	if (PawnOwner && PlayerController)
	{
		FVector AimDir;
		FVector Location;
		FRotator Rotation;
		//获取 发射方向

//		PlayerController->GetPlayerViewPoint(Location, Rotation); Returns Player's Point of View For the AI this means the Pawn's 'Eyes' ViewPoint For a Human player, this means the Camera's ViewPoint
 	
		PlayerController->GetActorEyesViewPoint(Location, Rotation);
 		AimDir = Rotation.Vector();
//		HitResult = WeaponTrace(Location, AimDir*SniperWeaponData.WeaponRange);
// 		static FName weapontag = FName(TEXT("Weapontag"));
// 		FCollisionQueryParams TraceParams(weapontag, true, Instigator);
// 		GetWorld()->DebugDrawTraceTag = weapontag;
// 		GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location+AimDir*SniperWeaponData.WeaponRange, COLLISION_WEAPON, TraceParams);
// 		int32 ViewportX, ViewportY;
// 		PlayerController->GetViewportSize(ViewportX, ViewportY);
// 		if (PlayerController->DeprojectScreenPositionToWorld(ViewportX*0.5, ViewportY*0.5, Location, AimDir))
// 		{
// 			HitResult = WeaponTrace(Location, AimDir*SniperWeaponData.WeaponRange);
// 		}

//		UE_LOG(LogTemp, Warning, TEXT("Rotation: %s "), *AimDir.ToString());
		HitResult = WeaponTrace(Location, Location+ AimDir*SniperWeaponData.WeaponRange);

		ProcessSniperHit(HitResult, Location, AimDir);
	}
}

void AWeapon_sniper::OnStartTarget()
{
	if (USniperTargetWidget* TargetWidget = PawnOwner->GetTargetWidget())
	{
		TargetWidget->SetVisibility(ESlateVisibility::Visible);
	}
	if (Mesh1P && SniperViewMesh)
	{
		Mesh1P->SetVisibility(false);
		SniperViewMesh->SetVisibility(false);
	}
	if (PawnOwner)
	{
		PawnOwner->SetFOV(TargetFOV);
	}
}

void AWeapon_sniper::OnStopTarget()
{
	if (USniperTargetWidget* TargetWidget = PawnOwner->GetTargetWidget())
	{
		TargetWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (Mesh1P)
	{
		Mesh1P->SetVisibility(true);
		SniperViewMesh->SetVisibility(true);

	}
	if (PawnOwner)
	{
		PawnOwner->SetFOV(NormalFov);
		PawnOwner->SetIsTargeting(false);
	}
}

void AWeapon_sniper::SpawnImpactEffects(const FHitResult& Impact)
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

void AWeapon_sniper::ProcessSniperHit(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir)
{
	ProcessSniprHit_Confirm(Impact, Orign, ShootDir);
}

void AWeapon_sniper::ProcessSniprHit_Confirm(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir)
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

void AWeapon_sniper::DealDamage(const FHitResult& Impact, const FVector& ShootDir)
{
	FPointDamageEvent PointDamage;
	PointDamage.DamageTypeClass = SniperWeaponData.DamageType;
	PointDamage.HitInfo = Impact;
	PointDamage.ShotDirection = ShootDir;
	PointDamage.Damage = SniperWeaponData.damage;

	if (Impact.BoneName == FName(TEXT("head")))
	{
		PointDamage.Damage *= 2;
	}
	if (Impact.GetActor())
	{
		Impact.GetActor()->TakeDamage(PointDamage.Damage, PointDamage, PawnOwner->GetController(), this);
	}
}