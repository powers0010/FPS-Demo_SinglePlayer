// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUpItem_Quest.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Animation/AnimationAsset.h"
#include "ShooterCharacter.h"


APickUpItem_Quest::APickUpItem_Quest()
{
	bIsOpen = false;
}

void APickUpItem_Quest::OnCollisionBoxBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
//	Super::OnCollisionBoxBeginOverLap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!bIsOpen && OpenAnim)
	{
		AShooterCharacter* Player = Cast<AShooterCharacter>(OtherActor);
		OnPickUp(Player);
		Mesh->PlayAnimation(OpenAnim,false);
		bIsOpen = true;
		if (PickUpEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(PickUpEffect, RootComponent);
		}
	}
}

void APickUpItem_Quest::OnPickUp(AShooterCharacter* Player)
{
	if (Player)
	{
		Player->AddDoorKeyAmout();
		Player->GetPickUpTipWidget()->ShowPickUpItemTip(this, true);
	}
//	UE_LOG(LogTemp, Warning, TEXT("OnPickUp APickUpItem_Quest"));
}