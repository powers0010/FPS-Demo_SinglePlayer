// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUpTip.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/Engine/Public/TimerManager.h"



#define LOCTEXT_NAMESPACE "Your Namespace" 

UTextBlock* UPickUpTip::GetTipText()
{
	return TipsText;
}

void UPickUpTip::ShowPickUpItemTip(APickUpItem* PickUpItem ,bool bSucced)
{
	GetWorld()->GetTimerManager().SetTimer(Timer_HideTip, this, &UPickUpTip::HideTipsText, 5.f, false);

	if (bSucced && PickUpItem)
	{
//		FText TipText = FText::Format(LOCTEXT("PickUpTipFormat", "You Get: {0} ! "), PickUpItem->GetItemName());
		TipsText->SetText(PickUpItem->GetItemName());
		TipsText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TipsText->SetText(PickUpItem->GetFailedText());
		TipsText->SetVisibility(ESlateVisibility::Visible);
	}
// 	switch (PickUpItemCategory)
// 	{
// 	case EPickUpItemCategory::Weapon:
// 		if (TipsText)
// 		{
// 			FText TipText = FText::Format(LOCTEXT("PickUpTipFormat", "You Get: {0} ! "), ItemName);
// 			TipsText->SetText(TipText);
// 			TipsText->SetVisibility(ESlateVisibility::Visible);
// 		}
// 		
// 		break;
// 	case EPickUpItemCategory::Clip:
// 		if (TipsText)
// 		{
// 			FText TipText = FText::Format(LOCTEXT("PickUpTipFormat", "You Get {0} Clip!"), FText::AsNumber(PickUpItemInfo.ClipAmount));
// 			TipsText->SetText(TipText);
// 			TipsText->SetVisibility(ESlateVisibility::Visible);
// 		}
// 		break;
// 	case EPickUpItemCategory::IncreaseHealth:
// 		if (TipsText)
// 		{
// 			FText TipText = LOCTEXT("PickUpTipFormat", "Health Up");
// 			TipsText->SetText(TipText);
// 			TipsText->SetVisibility(ESlateVisibility::Visible);
// 		}
// 		break;
// 	case EPickUpItemCategory::Buff:
// 		break;
// 	default:
// 		break;
// 	}
}

void UPickUpTip::HideTipsText()
{
	if (TipsText)
	{
		TipsText->SetVisibility(ESlateVisibility::Hidden);
	}
}


#undef LOCTEXT_NAMESPACE 