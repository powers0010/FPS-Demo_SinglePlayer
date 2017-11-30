// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "ShooterGame.h"
#include "ShooterCharacter.h"
#include "ShooterPlayerController.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ExplosionEffect.h"



// Sets default values
AWeapon::AWeapon() : AmmoCount(50)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	RootComponent = Mesh1P;
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh1P->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh1P->SetCollisionResponseToChannels(ECR_Ignore);
	Mesh1P->CastShadow = false;
	Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh1P->SetRelativeTransform(FTransform(FRotator(0.f, 0.f, -90.f)));

	FireSound = nullptr;
	PawnOwner = nullptr;


}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	ClipCount = WeaponConfig.ClipAmount;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::AttachWeaponToPawn()
{
	if (PawnOwner)
	{
		USkeletalMeshComponent* PawnMesh1P = PawnOwner->GetMesh1P();
		/*FName WeaponPoint = PawnOwner->GetWeaponAttachPoint();*/
		if (PawnMesh1P)
		{
			Mesh1P->SetHiddenInGame(false);
			Mesh1P->AttachToComponent(PawnMesh1P, FAttachmentTransformRules::KeepRelativeTransform, WeaponAttachPoint);
		}
	}
}

void AWeapon::DetachWeaponFromPawn()
{
	Mesh1P->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh1P->SetHiddenInGame(true);
}


void AWeapon::SetPawnOwner(AShooterCharacter* Pawn)
{
	if (Pawn)
	{
		PawnOwner = Pawn;
	}
}

FVector AWeapon::GetAdjustAim()
{
	FVector AimDir = FVector::ZeroVector;
	AShooterPlayerController* PlayerController = Instigator ? Cast<AShooterPlayerController>(Instigator->GetController()) : nullptr;
	if (PlayerController)
	{
		FVector Location;
		FRotator Rotation;
		//获取 发射方向
		PlayerController->GetPlayerViewPoint(Location, Rotation);
		AimDir = Rotation.Vector();
	}
//	UE_LOG(LogTemp, Warning, TEXT("AimDir: %s"), *AimDir.ToString());
	return AimDir;
}

FHitResult AWeapon::WeaponTrace(FVector TraceFrom, FVector TraceTo) const
{
	static FName weapontag = FName(TEXT("Weapontag"));
	FCollisionQueryParams TraceParams(weapontag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
/*	GetWorld()->DebugDrawTraceTag = weapontag;*/
	FHitResult HitResult(ForceInit);
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return HitResult;
}

FVector AWeapon::GetMuzzleLocation()
{
	if (Mesh1P)
	{
		return Mesh1P->GetSocketLocation(MuzzleAttachPoint);
	}
	return FVector::ZeroVector;
}


void AWeapon::OnStartFire()
{
	if (AmmoCount > 0)
	{
		SimulateWeaponFire();
		FireWeapon();
		AmmoCount--;
	}
}

void AWeapon::OnStopFire()
{

}
void AWeapon::OnStartTarget()
{

}

void AWeapon::OnStopTarget()
{

}

void AWeapon::OnReload()
{
	AmmoCount = FMath::Min(AmmoCount+WeaponConfig.AmmoPerClip ,GetMaxAmmoAmount());
	ClipCount--;
}

void AWeapon::OnEquip()
{

}

bool AWeapon::CanReload()
{
	return AmmoCount < GetMaxAmmoAmount() && ClipCount>0;
}

void AWeapon::SimulateWeaponFire()
{
	if (FireSound)
	{
		PlayWeaponSound(FireSound);
	}
	//开火动画
	if (!bPlayingFiringAnim)
	{
/*		bPlayingFiringAnim = true;*/
		PlayMontageAnimation(FireAnim);
	}
	if (FireCameraShake)
	{
		AShooterPlayerController* PC = PawnOwner != nullptr ? Cast<AShooterPlayerController>(PawnOwner->Controller) : nullptr;
		if (PC && PC->IsLocalController())
		{
			PC->ClientPlayCameraShake(FireCameraShake);
		}
	}
}

void AWeapon::FireWeapon()
{

}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue* FireSound)
{
	UAudioComponent* AC = nullptr;
	if (FireSound)
	{
		AC = UGameplayStatics::SpawnSoundAttached(FireSound, PawnOwner->GetRootComponent());
	}
	return AC;
}

int32 AWeapon::GetCurrentAmmoAmount() const
{
	return AmmoCount;
}

int32 AWeapon::GetMaxAmmoAmount() const
{
	return GetClass()->GetDefaultObject<AWeapon>()->AmmoCount;
}

int32 AWeapon::GetClipAmount() const
{
	return ClipCount;
}

float AWeapon::GetFireCD()
{
	return WeaponConfig.FireCD;
}

UAnimMontage* AWeapon::GetEquipAnim()
{
	return EquipAnim.AnimP;
}

UAnimMontage* AWeapon::GetReloadAnim()
{
	return ReloadAnim.AnimP;
}

USkeletalMeshComponent* AWeapon::GetMesh1P()
{
	return Mesh1P;
}

USoundCue* AWeapon::GetEquipSound()
{
	return EquipSound;
}

USoundCue* AWeapon::GetReloadSound()
{
	return ReloadSound;
}

float AWeapon::PlayMontageAnimation(const FWeaponAnim Animation)
{
	float during = 0.f;
	if (PawnOwner)
	{
		UAnimMontage* UseMontage = Animation.AnimP;
		if (UseMontage)
		{
			during = PawnOwner->PlayAnimMontage(UseMontage);
		}
	}
	return during;
}

void AWeapon::StopMontageAnimation(const FWeaponAnim Animation)
{
	if (PawnOwner)
	{
		UAnimMontage* UseMontage = Animation.AnimP;
		if (UseMontage)
		{
			PawnOwner->StopAnimMontage(UseMontage);
		}
	}
}

void AWeapon::OnGetClip(int32 Amount)
{
	ClipCount += Amount;
}