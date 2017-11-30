// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp/PickUpItem.h"
#include "PickUpItem_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API APickUpItem_Weapon : public APickUpItem
{
	GENERATED_BODY()
	
public:
	virtual void OnPickUp(AShooterCharacter* Player);

	virtual void OnRespawn();
protected:
	UPROPERTY(EditDefaultsOnly, Category = Default)
	TSubclassOf<class AWeapon> WeaponClass;
	
};
