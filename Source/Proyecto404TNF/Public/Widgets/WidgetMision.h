// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMision.generated.h"

class UButton;
class ANPCMision;

UCLASS()
class PROYECTO404TNF_API UWidgetMision : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY()
	ANPCMision* NPCVinculado;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Entregar;
	
	UFUNCTION()
	void OnButtonEntregarClicked();
};
