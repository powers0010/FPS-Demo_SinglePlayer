// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUpItem_Weapon.h"
#include "ShooterCharacter.h"
#include "PickUpTip.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Weapon.h"




void APickUpItem_Weapon::OnPickUp(AShooterCharacter* Player)
{
	Super::OnPickUp(Player);
	if (!bIsActive) { return; }

	if (Player)
	{
		if (WeaponClass && Player->OnGetWeapon(WeaponClass))
		{
			Player->GetPickUpTipWidget()->ShowPickUpItemTip(this , true);

			//
			AfterPickUpSucceed();
		}
		else
		{
			Player->GetPickUpTipWidget()->ShowPickUpItemTip(this, false);
		}
	}
}

void APickUpItem_Weapon::OnRespawn()
{

}