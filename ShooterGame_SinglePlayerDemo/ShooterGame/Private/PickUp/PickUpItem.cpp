// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUpItem.h"
#include "ShooterCharacter.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Weapon.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "PickUpTip.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"




// Sets default values
APickUpItem::APickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);

	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidget->SetupAttachment(RootComponent);

	PickUpParticleCom = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PickUpParticleCom"));
	PickUpParticleCom->bAutoActivate = false;
	PickUpParticleCom->bAutoDestroy = false;
	PickUpParticleCom->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
	bIsActive = true;
	bCanRespawn = false;
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APickUpItem::OnCollisionBoxBeginOverLap);
	}

	if (ActiveEffect && PickUpParticleCom)
	{
		PickUpParticleCom->SetTemplate(ActiveEffect);
		PickUpParticleCom->SetActive(true);
	}
}

void APickUpItem::OnCollisionBoxBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	AShooterCharacter* Player = Cast<AShooterCharacter>(OtherActor);
 	if (Player)
 	{
		OnPickUp(Player);
// 		switch (PickUpItemCategory)
// 		{
// 		case EPickUpItemCategory::Weapon:
// 			break;
// 		case EPickUpItemCategory::Clip:
// 			if (PickUpItemInfo.ClipAmount>0 && Player->OnGetClip(PickUpItemInfo.ClipAmount))
// 			{	
// 				Player->GetPickUpTipWidget()->ShowPickUpItemTip(EPickUpItemCategory::Clip, PickUpItemInfo, ItemName);
// 				Destroy();
// 			}
// 			break;
// 		case EPickUpItemCategory::IncreaseHealth:
// 			if (PickUpItemInfo.IncreaseHealth>0 && Player->OnIncreaseHealth(PickUpItemInfo.IncreaseHealth))
// 			{
// 				Player->GetPickUpTipWidget()->ShowPickUpItemTip(EPickUpItemCategory::IncreaseHealth, PickUpItemInfo, ItemName);
// 				Destroy();
// 			}
// 			break;
// 		case EPickUpItemCategory::Buff:
// 			break;
// 		case EPickUpItemCategory::QuestItems:
// 			break;
// 		case EPickUpItemCategory::WorldStatic:
// 			break;
// 		default:
// 			break;
// 		}
// 		
 	}
}

void APickUpItem::ShowItemInfoWidget()
{
	if (PickUpWidget && true== PickUpWidget->bHiddenInGame )
	{
		PickUpWidget->SetHiddenInGame(false);
		//set highlight
		if (Mesh)
		{
			Mesh->SetRenderCustomDepth(true);
		}
		GetWorld()->GetTimerManager().SetTimer(Timer_HideItemInfo,this , &APickUpItem::HideItemInfoWidget, 2.f , false);
	}

}

void APickUpItem::HideItemInfoWidget()
{
	if (PickUpWidget)
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer_HideItemInfo);
		PickUpWidget->SetHiddenInGame(true);
	}
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void APickUpItem::OnFocusOn()
{
	if (bIsActive && true  ==  PickUpWidget->bHiddenInGame )
	{
		ShowItemInfoWidget();
//		UE_LOG(LogTemp, Warning, TEXT("OnFocusOn"));
	}

}

void APickUpItem::OnPickUp(AShooterCharacter* Player)
{
//	UE_LOG(LogTemp, Warning, TEXT("OnPickUp"));
}

bool APickUpItem::GetIsActive()
{
	return bIsActive;
}

void APickUpItem::AfterPickUpSucceed()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, PickupSound, GetActorLocation());
	}
	
	bIsActive = false;
	Mesh->SetHiddenInGame(true);
	PickUpWidget->SetHiddenInGame(true);
	PickUpParticleCom->SetActive(false);

	GetWorld()->GetTimerManager().SetTimer(Timer_ForRespawn, this, &APickUpItem::OnRespawn, RespawnTime, false);
}

void APickUpItem::OnRespawn()
{
//	UE_LOG(LogTemp, Warning, TEXT("OnRespawn"));
	if (bCanRespawn)
	{
		bIsActive = true;
		Mesh->SetHiddenInGame(false);
		PickUpParticleCom->SetActive(true);
		if (RespawningEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(RespawningEffect, RootComponent);
		}
	}
	GetWorld()->GetTimerManager().ClearTimer(Timer_ForRespawn);
}

FText APickUpItem::GetItemName()
{
	return ItemName;
}

FText APickUpItem::GetFailedText()
{
	return FailedToPickUp;
}

USkeletalMeshComponent* APickUpItem::GetMesh()
{
	return Mesh;
}