// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUpItem_Health.h"
#include "ShooterCharacter.h"
#include "PickUpTip.h"


void APickUpItem_Health::OnCollisionBoxBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AShooterCharacter* Player = Cast<AShooterCharacter>(OtherActor);
	if (Player)
	{
		OnPickUp(Player);
	}
}

void APickUpItem_Health::OnPickUp(AShooterCharacter* Player)
{
	Super::OnPickUp(Player);
	if (!bIsActive) { return; }
	if (Player)
	{
		if (Player->OnIncreaseHealth(HealthAmount))
		{
			Player->GetPickUpTipWidget()->ShowPickUpItemTip(this, true);

			//
			AfterPickUpSucceed();
		}
		else
		{
			Player->GetPickUpTipWidget()->ShowPickUpItemTip(this, false);
		}
	}
}
