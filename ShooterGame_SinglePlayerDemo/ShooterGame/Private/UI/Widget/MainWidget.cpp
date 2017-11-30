// Fill out your copyright notice in the Description page of Project Settings.

#include "MainWidget.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Image.h"


void UMainWidget::InitMainWidget(UTextBlock* _DoorkeyNumText, UImage* _BeHitImage)
{
	DoorkeyNumText = nullptr;
	BeHitImage = nullptr;
	if (_DoorkeyNumText)
	{
		DoorkeyNumText = _DoorkeyNumText;
	}
	if (_BeHitImage)
	{
		BeHitImage = _BeHitImage;
	}
}

void UMainWidget::UpdataSetDoorKeyAmout(int32 amount)
{
	if (DoorkeyNumText)
	{
		DoorkeyNumText->SetText(FText::FromString(FString::FromInt(amount)));
	}
}

void UMainWidget::AddBeHitImageToView()
{
	if (BeHitImage && BeHitImage->Visibility== ESlateVisibility::Hidden && BeAttackedTexture.Num()>0)
	{
		BeHitImage->SetBrushFromTexture(BeAttackedTexture[FMath::RandRange(0, BeAttackedTexture.Num() - 1)]);
		BeHitImage->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(Timer_ShowBeAttackedImage, this, &UMainWidget::RemoveBeHitImageFromView, 0.5f, false);
//		UE_LOG(LogTemp, Warning, TEXT("AddBeHitImageToView"));
	}
}

void UMainWidget::RemoveBeHitImageFromView()
{
	if (BeHitImage)
	{
		BeHitImage->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(Timer_ShowBeAttackedImage);
	}
}