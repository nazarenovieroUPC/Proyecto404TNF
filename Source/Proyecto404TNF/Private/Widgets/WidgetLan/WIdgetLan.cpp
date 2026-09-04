// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WidgetLan/WIdgetLan.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void UWIdgetLan::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Button_Hostear->OnClicked.AddDynamic(this, &UWIdgetLan::OnButtonHostearClicked);
	Button_Unirse->OnClicked.AddDynamic(this, &UWIdgetLan::OnButtonUnirseClicked);
	
	FInputModeUIOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
}

void UWIdgetLan::OnButtonHostearClicked()
{
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level, true, TEXT("listen"));
}

void UWIdgetLan::OnButtonUnirseClicked()
{
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);
	
	const FString IP = EditableTextBox_DireccionIP->GetText().ToString();
	UGameplayStatics::OpenLevel(this, *IP);
}
