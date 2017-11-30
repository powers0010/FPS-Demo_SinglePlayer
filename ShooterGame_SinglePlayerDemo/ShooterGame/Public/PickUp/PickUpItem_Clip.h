// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp/PickUpItem.h"
#include "PickUpItem_Clip.generated.h"

/**
 * 
 */

class AWeapon;

UCLASS()
class SHOOTERGAME_API APickUpItem_Clip : public APickUpItem
{
	GENERATED_BODY()

public:
	virtual void OnPickUp(AShooterCharacter* Player);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Default)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default)
	int32 ClipAmount;
	
};
