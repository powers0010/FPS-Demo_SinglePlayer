// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterImpactEffect.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ShooterType.h"
#include "Runtime/Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"


// Sets default values
AShooterImpactEffect::AShooterImpactEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bAutoDestroyWhenFinished = true;
}

// Called when the game starts or when spawned
void AShooterImpactEffect::BeginPlay()
{
	Super::BeginPlay();
	Destroy();
}

void AShooterImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//特效
	UPhysicalMaterial*  HitPhysMat = HitResult.PhysMaterial.Get();
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);

	UParticleSystem* ImpactEffect = GetImpactEffect(HitSurfaceType);
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, GetActorLocation(), GetActorRotation());
	}

	//音效
	USoundCue* ImpactSount = GetImpactSount(HitSurfaceType);
	if (ImpactSount)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSount, GetActorLocation());
	}

	//贴花
// 	if (DefaultDecal.DecalMaterial)
// 	{
// 		FRotator DecalRotation = HitResult.ImpactNormal.Rotation();
// 		DecalRotation.Roll = FMath::RandRange(-180.f, 180.f);
// 		UGameplayStatics::SpawnDecalAttached(DefaultDecal.DecalMaterial, FVector(DefaultDecal.DecalSize, 1.f, DefaultDecal.DecalSize),
// 			HitResult.GetComponent(), HitResult.BoneName, HitResult.ImpactPoint, DecalRotation, EAttachLocation::KeepWorldPosition, DefaultDecal.LifeSpan);
// 		//			UGameplayStatics::SpawnDecalAtLocation(this, DefaultDecal.DecalMaterial, FVector(DefaultDecal.DecalSize, DefaultDecal.DecalSize, DefaultDecal.DecalSize), HitResult.ImpactPoint, DecalRotation, DefaultDecal.LifeSpan);
// 	}
}

void AShooterImpactEffect::SetHitResult(const FHitResult& _HitResult)
{
	HitResult = _HitResult;
}

UParticleSystem* AShooterImpactEffect::GetImpactEffect(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	UParticleSystem* ImpacrEffect = nullptr;
	switch (SurfaceType)
	{
	case SHOOTER_SURFACE_Concrete:	ImpacrEffect = ConcreteFX;		/*UE_LOG(LogTemp, Warning, TEXT("Concrete"))*/; break;
	case SHOOTER_SURFACE_Dirt:		ImpacrEffect = DirtFX;			/*UE_LOG(LogTemp, Warning, TEXT("Dirt"));*/ break;
	case SHOOTER_SURFACE_Water:		ImpacrEffect = WaterFX;			/*UE_LOG(LogTemp, Warning, TEXT("Water"));*/ break;
	case SHOOTER_SURFACE_Metal:		ImpacrEffect = MetalFX;			/*UE_LOG(LogTemp, Warning, TEXT("Metal"));*/ break;
	case SHOOTER_SURFACE_Wood:		ImpacrEffect = WoodFX;			/*UE_LOG(LogTemp, Warning, TEXT("Wood")); */break;
	case SHOOTER_SURFACE_Grass:		ImpacrEffect = GrassFX;			/*UE_LOG(LogTemp, Warning, TEXT("Grass")); */break;
	case SHOOTER_SURFACE_Glass:		ImpacrEffect = GlassFX;			/*UE_LOG(LogTemp, Warning, TEXT("Glass"));*/ break;
	case SHOOTER_SURFACE_Flesh:		ImpacrEffect = FleshFX;			/*UE_LOG(LogTemp, Warning, TEXT("Flesh")); */break;
	default:						ImpacrEffect = DefaultFX;		/*UE_LOG(LogTemp, Warning, TEXT("Default")); */break;
	}
	return ImpacrEffect;
}

USoundCue* AShooterImpactEffect::GetImpactSount(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	USoundCue* ImpacrSount = nullptr;
	switch (SurfaceType)
	{
	case SHOOTER_SURFACE_Concrete:	ImpacrSount = ConcreteSound;		break;//混凝土
	case SHOOTER_SURFACE_Dirt:		ImpacrSount = DirtSound;			break;//混凝土
	case SHOOTER_SURFACE_Water:		ImpacrSount = WaterSound;			break;//混凝土
	case SHOOTER_SURFACE_Metal:		ImpacrSount = MetalSound;			break;//混凝土
	case SHOOTER_SURFACE_Wood:		ImpacrSount = WoodSound;			break;//混凝土
	case SHOOTER_SURFACE_Grass:		ImpacrSount = GrassSound;			break;//混凝土
	case SHOOTER_SURFACE_Glass:		ImpacrSount = GlassSound;			break;//混凝土
	case SHOOTER_SURFACE_Flesh:		ImpacrSount = FleshSound;			break;//混凝土
	default:						ImpacrSount = DefaultSound;			break;
	}
	return ImpacrSount;
}