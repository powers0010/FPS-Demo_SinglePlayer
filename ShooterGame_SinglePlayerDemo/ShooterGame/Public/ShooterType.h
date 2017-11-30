// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

 /** keep in sync with ShooterImpactEffect */
UENUM()
namespace EShooterPhysMaterialType
{
	enum Type
	{
		Unknown,
		Concrete,	//混凝土
		Dirt,		//泥土
		Water,		//水
		Metal,		//金属
		Wood,		//木头
		Grass,		//草地
		Glass,		//玻璃
		Flesh,		//肉体
	};
}

#define SHOOTER_SURFACE_Default		SurfaceType_Default
#define SHOOTER_SURFACE_Concrete	SurfaceType1
#define SHOOTER_SURFACE_Dirt		SurfaceType2
#define SHOOTER_SURFACE_Water		SurfaceType3
#define SHOOTER_SURFACE_Metal		SurfaceType4
#define SHOOTER_SURFACE_Wood		SurfaceType5
#define SHOOTER_SURFACE_Grass		SurfaceType6
#define SHOOTER_SURFACE_Glass		SurfaceType7
#define SHOOTER_SURFACE_Flesh		SurfaceType8