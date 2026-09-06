// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WIdgetLan.generated.h"

class UEditableTextBox;
class UButton;
/**
 * 
 */
UCLASS()
class PROYECTO404TNF_API UWIdgetLan : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	
private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Hostear;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Unirse;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_DireccionIP;
	
	UPROPERTY(EditDefaultsOnly, Category = "Configuracion de Red")
	TSoftObjectPtr<UWorld> Level;
	
	UFUNCTION()
	void OnButtonHostearClicked();
	UFUNCTION()
	void OnButtonUnirseClicked();
};
