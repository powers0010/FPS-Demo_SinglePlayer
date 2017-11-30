// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickUpItem.h"
#include "PickUpTip.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class SHOOTERGAME_API UPickUpTip : public UUserWidget
{
	GENERATED_BODY()
public:
	UTextBlock* GetTipText();

	void ShowPickUpItemTip(APickUpItem* PickUpItem, bool bSucced);

	void HideTipsText();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = Text)
	UTextBlock* TipsText;
	
	FTimerHandle Timer_HideTip;
};
