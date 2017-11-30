// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Weapon_sniper.generated.h"

/**
 * 
 */

UCLASS()
class SHOOTERGAME_API AWeapon_sniper : public AWeapon
{
	GENERATED_BODY()

public:
	AWeapon_sniper();

	virtual void FireWeapon() override;

	virtual void SimulateWeaponFire() override;

	void OnStartFire();

	void OnStopFire();

	void FindHitResult(FHitResult& HitResult);

	virtual void OnStartTarget();

	virtual void OnStopTarget();

	void SpawnImpactEffects(const FHitResult& Impact);

	void ProcessSniperHit(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir);

	void ProcessSniprHit_Confirm(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir);

	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);

protected:
	UPROPERTY(EditDefaultsOnly, Category = FOV)
	float TargetFOV;

	UPROPERTY(EditDefaultsOnly, Category = FOV)
	float NormalFov;

	UPROPERTY(EditDefaultsOnly, category = WeaponData)
		FSniperWeaponData SniperWeaponData;

	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SniperViewMesh;		//Ãé×¼¾µ

	UPROPERTY(EditDefaultsOnly, category = Impact)
	TSubclassOf<class AShooterImpactEffect> ImpactEffectActor;
	
};
