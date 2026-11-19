// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BridgeInfoWidget.h"

#include "Components/TextBlock.h"

void UBridgeInfoWidget::SetInfoText(const FText& NewText)
{
	if (InfoText)
	{
		InfoText->SetText(NewText);
	}
}
