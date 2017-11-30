// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"


class AWeapon;
class USniperTargetWidget;
class UPickUpTip;
class APickUpItem;
class UMainWidget;


UCLASS()
class SHOOTERGAME_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//更新相机位置
	void OnCameraUpdata(const FVector& CameraLocation,const FRotator& CameraRotation);

	//初始化组件 调用
	virtual void PostInitializeComponents();

	//伤害接口
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate  = 1.f , FName StartSectionName  = NAME_None ) override;

	virtual void StopAnimMontage(class UAnimMontage* AnimMontage  = NULL ) override;

	virtual bool OnIncreaseHealth(float _in);

	virtual bool OnGetWeapon(TSubclassOf<AWeapon> NewWeaponClass);

	virtual bool OnGetClip(int32 Amount, TSubclassOf<AWeapon> WeaponClass);

	virtual void Die(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	void SimulateBeAttacked();

	USkeletalMeshComponent* GetPawnMesh();

	//控制输入
	void MoveForward(float value);
	void MoveRight(float value);
	void OnStartTarget();
	void OnEndTarget();
	void OnStartFire();
	void OnStopFire();
	void OnReload();
	void OnEquip();
	
	//获取 变量的接口
	USkeletalMeshComponent* GetMesh1P();

	FName GetWeaponAttachPoint() const;

	UFUNCTION(BlueprintCallable , Category = Weapon)
	FRotator GetAimOffsets() const;

	UFUNCTION(BlueprintCallable, Category = Weapon)
		bool GetIsTargeting() const;
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SetIsTargeting(bool NewIsTarget);

	int32 GetCurHealth() const;

	int32 GetMaxHealth() const;

	AWeapon* GetCurrentWeapon();

	bool CanFire();

	bool IsAlive();

	void TraceForPickUpItem(/*FHitResult& HitResult*/);

	void OnPickUp();

	USniperTargetWidget* GetTargetWidget() const;

	UPickUpTip* GetPickUpTipWidget() const;

	void SetFOV(float _FOV);

	UFUNCTION(BlueprintCallable, Category = "Widget")
		void SetMainWidget(UMainWidget* widget);

	UFUNCTION(BlueprintCallable, Category = "Widget")
		UMainWidget* GetMainWidget();

	UFUNCTION(BlueprintCallable, Category = "Level")
		void AddDoorKeyAmout();

	UFUNCTION(BlueprintCallable, Category = "Level")
		void SubtractDoorKeyAmout();

	UFUNCTION(BlueprintCallable, Category = "Level")
		int32 GetDoorKeyAmout();
protected:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UWeaponSystemComponent* WeaponSysCom;

	UPROPERTY(EditDefaultsOnly, Category = MainWidget)
		UMainWidget* MainWidget;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		FName WeaponAttachPoint;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		bool bIsTargeting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
		float Health;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		float RangeOfFocusOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Target)
		USniperTargetWidget* SniperTargetWideget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickUp)
		UPickUpTip* PickUpTip;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
		TSubclassOf<UCameraShake> BeHitCameraShake;

	APickUpItem* FocusOnItem;

	int32 DoorKeyAmount;
};
