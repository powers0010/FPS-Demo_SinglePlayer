// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponSystemComponent.generated.h"


class AWeapon;
class AShooterCharacter;
class UAnimMontage;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERGAME_API UWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponSystemComponent();

	AWeapon* GetCurrentWeapon();

	void SetPawnOwner(AShooterCharacter* Owner);

	void CreateNewWeapon(TSubclassOf<AWeapon> NewWeaponClass);

	void ChangeWeaponAndAttachToPawn(int32 WeaponIndexInInventory);

	void DetachCuttentWeaponFromPawn();

	void CreateDefaultWeaponAtGameStart();

	void OnStartFire();

	void OnStopFire();

	void OnEquip();

	void OnReload();

	void OnStartTarget();

	void OnStopTarget();

	float PlayMontageAnimation(UAnimMontage* Animation);

	void StopMontageAnimation(UAnimMontage* Animation);

	void AddToInventory(TSubclassOf<AWeapon> NewWeapon);

	virtual bool OnGetWeapon(TSubclassOf<AWeapon> NewWeaponClass);

	virtual bool OnGetClip(int32 Amount , TSubclassOf<AWeapon> WeaponClass);

	bool IsInInventory(TSubclassOf<AWeapon> WeaponClass , int32& Index);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AShooterCharacter* PawnOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = Weapon)
	TArray<TSubclassOf<AWeapon>> Inventory;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Weapon)
	TArray<AWeapon*> WeaponCreatedInInventory;

	AWeapon* CurrentWeapon;

	int32 CurrentWeaponIndexInInventory;

	float LastWeaponFireTime;

	bool bCanFire;

	bool CanFire();

	bool bCanReload;

	bool CanReload();

	bool bCanEquip;

	bool CanEquip();
	
	FTimerHandle Timer_OnEquipFinish;

	FTimerHandle Timer_OnReloadFinish;

	void OnEquipFinish();

	void OnReloadFinish();
		
	
};
