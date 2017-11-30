// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UImage;

UCLASS()
class SHOOTERGAME_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Init")
	void InitMainWidget(UTextBlock* _DoorkeyNumText , UImage* _BeHitImage);
	
	void UpdataSetDoorKeyAmout(int32 amount);

	void AddBeHitImageToView();

	void RemoveBeHitImageFromView();
protected:
	UTextBlock* DoorkeyNumText;

	UImage* BeHitImage;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
		TArray<UTexture2D*> BeAttackedTexture;

	FTimerHandle Timer_ShowBeAttackedImage;
};
