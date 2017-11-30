// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp/PickUpItem.h"
#include "PickUpItem_Health.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API APickUpItem_Health : public APickUpItem
{
	GENERATED_BODY()
public:
	virtual void OnPickUp(AShooterCharacter* Player);

	virtual void OnCollisionBoxBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default)
	int32 HealthAmount;
	
	
	
};
