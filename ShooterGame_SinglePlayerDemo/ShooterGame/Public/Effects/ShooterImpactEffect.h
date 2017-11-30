// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterImpactEffect.generated.h"


class UParticleSystem;
class USoundCue;
class UMaterial;

//贴花
USTRUCT()
struct FDecalData
{
	GENERATED_USTRUCT_BODY()

		/** material */
		UPROPERTY(EditDefaultsOnly, Category = Decal)
		UMaterial* DecalMaterial;

	/** quad size (width & height) */
	UPROPERTY(EditDefaultsOnly, Category = Decal)
		float DecalSize;

	/** lifespan */
	UPROPERTY(EditDefaultsOnly, Category = Decal)
		float LifeSpan;

	/** defaults */
	FDecalData()
		: DecalSize(256.f)
		, LifeSpan(10.f)
	{
	}
};


UCLASS()
class SHOOTERGAME_API AShooterImpactEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterImpactEffect();

	virtual void PostInitializeComponents() override;

	void SetHitResult(const FHitResult& _HitResult);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FHitResult HitResult;
	//--------------------------【粒子特效-】------------------------
	//默认 
	UPROPERTY(EditDefaultsOnly, Category = Particle)
		UParticleSystem* DefaultFX;

	//混凝土
	UPROPERTY(EditDefaultsOnly, Category = Particle)
		UParticleSystem* ConcreteFX;

	//泥土
	UPROPERTY(EditDefaultsOnly, Category = Particle)
		UParticleSystem* DirtFX;

	//水
	UPROPERTY(EditDefaultsOnly, Category = Particle)
		UParticleSystem* WaterFX;

	//金属
	UPROPERTY(EditDefaultsOnly, Category = Particle)
		UParticleSystem* MetalFX;

	//木头
	UPROPERTY(EditDefaultsOnly, Category = Particle)
		UParticleSystem* WoodFX;

	//草
	UPROPERTY(EditDefaultsOnly, Category = Particle)
		UParticleSystem* GrassFX;
	//玻璃
	UPROPERTY(EditDefaultsOnly, Category = Particle)
		UParticleSystem* GlassFX;

	//肉体
	UPROPERTY(EditDefaultsOnly, Category = Particle)
		UParticleSystem* FleshFX;

	//----------------------【开枪音效】-------------------------
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* DefaultSound;

	//混凝土
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* ConcreteSound;

	//泥土
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* DirtSound;

	//水
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* WaterSound;

	//金属
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* MetalSound;

	//木头
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* WoodSound;

	//草
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* GrassSound;
	//玻璃
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* GlassSound;

	//肉体
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* FleshSound;

	//--------------------【贴花】---------------------------------
	UPROPERTY(EditDefaultsOnly, Category = Defaults)
	struct FDecalData DefaultDecal;

public:	

	UParticleSystem* GetImpactEffect(TEnumAsByte<EPhysicalSurface> SurfaceType) const;
	
	USoundCue* GetImpactSount(TEnumAsByte<EPhysicalSurface> SurfaceType) const;



	
	
};
