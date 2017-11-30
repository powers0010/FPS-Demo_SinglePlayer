// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Weapon_machine.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AWeapon_machine : public AWeapon
{
	GENERATED_BODY()
public:
	AWeapon_machine();

	virtual void SimulateWeaponFire() override;

	virtual void FireWeapon() override;
	
	virtual void OnStartFire();

	virtual void OnStopFire();
	
	virtual void OnReload();

	virtual void OnEquip();


	void FindHitResult(FHitResult& HitResult);

	void ProcessSniperHit(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir);

	void ProcessSniprHit_Confirm(const FHitResult& Impact, const FVector& Orign, const FVector& ShootDir);

	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);
protected:

	FTimerHandle Timer_OnStartFire;

	//用于存储 循环音效 与 循环粒子，以便destory
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* FireAudioCom;

	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* FireParticleCom;

	UPROPERTY(EditDefaultsOnly, category = Weapon)
	float WeaponRange;

	UPROPERTY(EditDefaultsOnly, category = Impact)
	TSubclassOf<class AShooterImpactEffect> ImpactEffectActor;

	void SpawnImpactEffects(const FHitResult& Impact);

	UPROPERTY(EditDefaultsOnly, category = WeaponData)
	FSniperWeaponData SniperWeaponData;
};
