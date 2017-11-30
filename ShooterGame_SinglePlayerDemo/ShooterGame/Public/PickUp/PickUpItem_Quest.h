// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp/PickUpItem.h"
#include "PickUpItem_Quest.generated.h"

/**
 * 
 */
class UAnimationAsset;

UCLASS()
class SHOOTERGAME_API APickUpItem_Quest : public APickUpItem
{
	GENERATED_BODY()
public:
	APickUpItem_Quest();

	virtual void OnCollisionBoxBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnPickUp(AShooterCharacter* Player) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Effects)
		UParticleSystem* PickUpEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = Anim)
		UAnimationAsset* OpenAnim;

	bool bIsOpen;
};
