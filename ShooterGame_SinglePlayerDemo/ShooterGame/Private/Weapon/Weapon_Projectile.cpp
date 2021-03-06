// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_Projectile.h"
#include "ShooterProjectile.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


void AWeapon_Projectile::FireWeapon()
{
	//初始化子弹方向 和 初始位置
	const FVector ShootDirection = GetAdjustAim();
	const FVector Origin = GetMuzzleLocation();

	FTransform SpawnTransform(ShootDirection.Rotation(), Origin);

	//延时生成子弹对象，设置子弹参数
	AShooterProjectile* Projectile = Cast<AShooterProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileConfig.ProjectileClass, SpawnTransform)) ;
	if (Projectile)
	{
		Projectile->Instigator = this->Instigator;	//向下传递Instigator
		Projectile->SetOwner(this);
		Projectile->InitializeVelocity(ShootDirection);
		//完成延时生成
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	}
}

void AWeapon_Projectile::ApplyWeaponConfig(FProjectileWeaponData& ProjectileData)
{
	ProjectileData = ProjectileConfig;
}