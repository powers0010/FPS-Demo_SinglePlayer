// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpItem.generated.h"

class AWeapon;
class AShooterCharacter;
class USoundCue;
class UParticleSystemComponent;

// UENUM(BlueprintType)
// enum class EPickUpItemCategory :uint8
// {
// 	Weapon,
// 	Clip,
// 	IncreaseHealth,
// 	Buff,
// 	QuestItems,
// 	WorldStatic
// };
// 
// USTRUCT(BlueprintType)
// struct FPickUpItemInfo
// {
// 	GENERATED_USTRUCT_BODY()
// 
// 	UPROPERTY(EditAnywhere, Category = PickUpItemInfo)
// 	TSubclassOf<AWeapon> NewWeaponClass;
// 
// 	UPROPERTY(EditAnywhere, Category = PickUpItemInfo)
// 	float IncreaseHealth;
// 
// 	UPROPERTY(EditAnywhere, Category = PickUpItemInfo)
// 	int32 ClipAmount;
// };

UCLASS()
class SHOOTERGAME_API APickUpItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpItem();

	UFUNCTION()
	virtual void OnCollisionBoxBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void OnFocusOn();

	//响应玩家拾取事件
	virtual void OnPickUp(AShooterCharacter* Player);

	void ShowItemInfoWidget();

	void HideItemInfoWidget();

	bool GetIsActive();

	void AfterPickUpSucceed();

	virtual void OnRespawn();

	FText GetItemName();

	FText GetFailedText();

	USkeletalMeshComponent* GetMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = PickUp, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(Category = PickUp, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox; 

	UPROPERTY(Category = PickUp, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickUpWidget;

	UPROPERTY(VisibleDefaultsOnly, Category = Effects)
		UParticleSystemComponent* PickUpParticleCom;
	//------------【粒子和音效】
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* ActiveEffect;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* RespawningEffect;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		USoundCue* PickupSound;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		USoundCue* RespawnSound;

	//-------------【变量】-------
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		float RespawnTime;

	UPROPERTY(VisibleAnywhere, Category = Pickup)
		bool bIsActive;

	UPROPERTY(EditAnywhere, Category = Pickup)
		bool bCanRespawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Text)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Text)
	FText ItemDescribe;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Text)
	FText FailedToPickUp;

// 	UPROPERTY(EditAnywhere, Category = PickUp)
// 	EPickUpItemCategory PickUpItemCategory;
// 
// 	UPROPERTY(EditAnywhere, Category = PickUp)
// 	FPickUpItemInfo PickUpItemInfo;

	FTimerHandle Timer_HideItemInfo;

	FTimerHandle Timer_ForRespawn;

};
