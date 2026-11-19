// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BridgeInfoWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROYECTO404TNF_API UBridgeInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetInfoText(const FText& NewText);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InfoText;
};
