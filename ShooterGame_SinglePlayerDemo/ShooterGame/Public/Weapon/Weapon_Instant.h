// Fill out your copyright notice in the Description page of Project Settings.	//及时伤害武器  锥形角

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Weapon_Instant.generated.h"

/**
 * 
 */

class AShooterImpactEffect;


USTRUCT()
struct FInstantWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, category = InstantWeaponData)
	float WeaponSpreed;

	UPROPERTY(EditDefaultsOnly, category = InstantWeaponData)
	float WeaponRange;

	UPROPERTY(EditDefaultsOnly, category = InstantWeaponData)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, category = InstantWeaponData)
	int32 damage;
};


UCLASS()
class SHOOTERGAME_API AWeapon_Instant : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void FireWeapon() override;

	void ProcessInstantHit(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir, int32 RandomSeed, float ReticleSpreed);

	void ProcessInstantHit_Confirm(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir, int32 RandomSeed, float ReticleSpreed);

	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);

	void SpawnTraceEffect(const FVector& EndPoint );
protected:
	UPROPERTY(EditDefaultsOnly, category = InstantWeaponData)
	FInstantWeaponData InstantWeaponConfig;

	UPROPERTY(EditDefaultsOnly, category = Particles)
	FName TrailTargetParam;		//特效目标设置 名称

	void SpawnImpactEffects(const FHitResult& Impact);

	UPROPERTY(EditDefaultsOnly, category = Impact)
	TSubclassOf<AShooterImpactEffect> ImpactEffectActor;


};
