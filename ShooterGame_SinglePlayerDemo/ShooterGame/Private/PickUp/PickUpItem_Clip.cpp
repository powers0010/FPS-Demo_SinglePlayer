// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUpItem_Clip.h"
#include "ShooterCharacter.h"
#include "PickUpTip.h"
#include "Weapon.h"


void APickUpItem_Clip::OnPickUp(AShooterCharacter* Player)
{
	Super::OnPickUp(Player);
	if (!bIsActive) { return; }

	if (Player)
	{
		if (WeaponClass && Player->OnGetClip(ClipAmount , WeaponClass))
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
