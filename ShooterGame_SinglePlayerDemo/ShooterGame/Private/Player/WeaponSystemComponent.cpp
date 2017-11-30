// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponSystemComponent.h"
#include "Weapon.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "ShooterCharacter.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"



// Sets default values for this component's properties
UWeaponSystemComponent::UWeaponSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CurrentWeapon = nullptr;
	CurrentWeaponIndexInInventory = 0;
	LastWeaponFireTime = -1.f;
	bCanFire = true;
	bCanReload = true;
	bCanEquip = true;
	// ...
}


// Called when the game starts
void UWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

void UWeaponSystemComponent::CreateNewWeapon(TSubclassOf<AWeapon> NewWeaponClass)
{
	if (NewWeaponClass)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(NewWeaponClass, SpawnParameter);
		if (NewWeapon)
		{
			NewWeapon->SetPawnOwner(PawnOwner);
			NewWeapon->Instigator = PawnOwner->Instigator;
			//				NewWeapon->AttachWeaponToPawn();
			NewWeapon->GetMesh1P()->SetHiddenInGame(true);
			WeaponCreatedInInventory.Add(NewWeapon);
		}
	}
}

void UWeaponSystemComponent::CreateDefaultWeaponAtGameStart()
{
	if (Inventory.Num()>0)
	{
		for (int32 i =0;i<Inventory.Num();i++)
		{
			CreateNewWeapon(Inventory[i]);
		}
//		ChangeWeaponAndAttachToPawn(CurrentWeaponIndexInInventory);
	}
	if (WeaponCreatedInInventory.Num()>0)
	{
		CurrentWeapon = WeaponCreatedInInventory[0];
		CurrentWeapon->GetMesh1P()->SetHiddenInGame(false);
		CurrentWeapon->AttachWeaponToPawn();
	}
}



void UWeaponSystemComponent::ChangeWeaponAndAttachToPawn(int32 WeaponIndexInInventory)
{
	if (PawnOwner && WeaponCreatedInInventory.IsValidIndex(WeaponIndexInInventory)/* && WeaponCreatedInInventory[WeaponIndexInInventory]*/)
	{
// 		if ( WeaponCreatedInInventory.IsValidIndex(WeaponIndexInInventory) && WeaponCreatedInInventory[WeaponIndexInInventory])
// 		{
		DetachCuttentWeaponFromPawn();
			CurrentWeapon = WeaponCreatedInInventory[WeaponIndexInInventory];
			CurrentWeapon->GetMesh1P()->SetHiddenInGame(false);
			CurrentWeapon->AttachWeaponToPawn();
/*		} */
// 		else
// 		{
// 			FActorSpawnParameters SpawnParameter;
// 			SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
// 			AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(Inventory[WeaponIndexInInventory], SpawnParameter);
// 			if (NewWeapon)
// 			{
// 				NewWeapon->SetPawnOwner(PawnOwner);
// 				NewWeapon->Instigator = PawnOwner->Instigator;
// 				NewWeapon->AttachWeaponToPawn();
// 
// 				CurrentWeapon = NewWeapon;
// 			}
// 		}
	}
}

void UWeaponSystemComponent::DetachCuttentWeaponFromPawn()
{
	if (CurrentWeapon)
	{
// 		if (CurrentWeaponIndexInInventory > WeaponCreatedInInventory.Max())
// 		{
// 			WeaponCreatedInInventory.SetNum(2 * WeaponCreatedInInventory.Max() , true);
// 		}
// 		if (nullptr == WeaponCreatedInInventory[CurrentWeaponIndexInInventory])
// 		{
// 			WeaponCreatedInInventory[CurrentWeaponIndexInInventory] = CurrentWeapon;
// 		}
// 		
		CurrentWeapon->DetachWeaponFromPawn();
	}
}

AWeapon* UWeaponSystemComponent::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void UWeaponSystemComponent::SetPawnOwner(AShooterCharacter* Owner)
{
	PawnOwner = Owner;
}

void UWeaponSystemComponent::OnStartFire()
{
	if (CanFire() && CurrentWeapon && (GetWorld()->TimeSeconds - LastWeaponFireTime > CurrentWeapon->GetFireCD()))
	{
		CurrentWeapon->OnStartFire();
		LastWeaponFireTime = GetWorld()->TimeSeconds;
	}
}

void UWeaponSystemComponent::OnStopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->OnStopFire();
	}
}

void UWeaponSystemComponent::OnEquip()
{
	if (CanEquip())
	{
		CurrentWeapon->OnEquip();

		//卸载武器 增加新武器
		DetachCuttentWeaponFromPawn();
		CurrentWeaponIndexInInventory++;
		CurrentWeaponIndexInInventory = CurrentWeaponIndexInInventory % WeaponCreatedInInventory.Num();
		ChangeWeaponAndAttachToPawn(CurrentWeaponIndexInInventory);

		//播放装备动画
		float AnimDurTime = PlayMontageAnimation(CurrentWeapon->GetEquipAnim());
		GetWorld()->GetTimerManager().SetTimer(Timer_OnEquipFinish, this, &UWeaponSystemComponent::OnEquipFinish, AnimDurTime, false);
//		GetWorldTimerManager().SetTimer(Timer_OnEquipFinish, this, &UWeaponSystemComponent::OnEquipFinish, AnimDurTime, false);

		//播放装备音效
		CurrentWeapon->PlayWeaponSound(CurrentWeapon->GetEquipSound());

		//重置开火时间、状态
		LastWeaponFireTime = 0.f;
		bCanFire = false;
		bCanReload = false;
		bCanEquip = false;
	}
}

void UWeaponSystemComponent::OnReload()
{
	if (CanReload())
	{
		//播放装弹动画
		float AnimDurTime = 0.5f;
		UAnimMontage* UseMontage = CurrentWeapon->GetReloadAnim();
		if (UseMontage)
		{
			AnimDurTime = PlayMontageAnimation(UseMontage);
		}
		GetWorld()->GetTimerManager().SetTimer(Timer_OnReloadFinish, this, &UWeaponSystemComponent::OnReloadFinish, AnimDurTime, false);

		//播放装弹音效
		CurrentWeapon->PlayWeaponSound(CurrentWeapon->GetReloadSound());

		//更新子弹数量和弹夹数量
		CurrentWeapon->OnReload();

		//重置状态
		bCanFire = false;
		bCanEquip = false;
		bCanReload = false;
	}
}

float UWeaponSystemComponent::PlayMontageAnimation(UAnimMontage* Animation)
{
	float during = 0.f;
	if (PawnOwner && CurrentWeapon && Animation)
	{
		during = PawnOwner->PlayAnimMontage(Animation);
	}
	return during;
}

void UWeaponSystemComponent::StopMontageAnimation(UAnimMontage* Animation)
{
	if (PawnOwner && CurrentWeapon && Animation)
	{
		PawnOwner->StopAnimMontage(Animation);
	}
}

bool UWeaponSystemComponent::CanFire()
{
	return PawnOwner && bCanFire && PawnOwner->CanFire();
}

bool UWeaponSystemComponent::CanReload()
{
	return PawnOwner && bCanReload &&CurrentWeapon&& CurrentWeapon->CanReload();
}

bool UWeaponSystemComponent::CanEquip()
{
	return bCanEquip && CurrentWeapon && Inventory.Num() > 1;
}

void UWeaponSystemComponent::OnEquipFinish()
{
	bCanFire = true;
	bCanReload = true;
	bCanEquip = true;
	GetWorld()->GetTimerManager().ClearTimer(Timer_OnEquipFinish);
//	UE_LOG(LogTemp, Warning, TEXT("OnEquipFinish"));
}

void UWeaponSystemComponent::OnReloadFinish()
{
	bCanFire = true;
	bCanEquip = true;
	bCanReload = true;
	UE_LOG(LogTemp, Warning, TEXT("OnReloadFinish"));
}

void UWeaponSystemComponent::AddToInventory(TSubclassOf<AWeapon> NewWeapon)
{
	if (NewWeapon)
	{
		Inventory.AddUnique(NewWeapon);
	}
}


void UWeaponSystemComponent::OnStartTarget()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->OnStartTarget();
	}
}

void UWeaponSystemComponent::OnStopTarget()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->OnStopTarget();
	}
}

bool UWeaponSystemComponent::OnGetWeapon(TSubclassOf<AWeapon> NewWeaponClass)
{
	if (NewWeaponClass)
	{
		int32 IsInInventory = Inventory.Find(NewWeaponClass);
		if (INDEX_NONE ==IsInInventory)
		{
			Inventory.Add(NewWeaponClass);
			CreateNewWeapon(NewWeaponClass);
			return true;
		}
	}
	return false;
}

bool UWeaponSystemComponent::OnGetClip(int32 Amount, TSubclassOf<AWeapon> WeaponClass)
{
	int32 Index=0;
	if (IsInInventory(WeaponClass, Index))
	{
		WeaponCreatedInInventory[Index]->OnGetClip(Amount);
		return true;
	}
	return false;
}

bool UWeaponSystemComponent::IsInInventory(TSubclassOf<AWeapon> WeaponClass , int32& Index)
{
	bool bFind = Inventory.Find(WeaponClass,Index);
// 	for (int i = 0; i< Inventory.Num();i++)
// 	{
// 		bFind = WeaponClass->IsChildOf(Inventory[i]);
// 		if (bFind)
// 		{
// 			Index = i;
// 			break;
// 		}
// 	}
	return bFind;

}
