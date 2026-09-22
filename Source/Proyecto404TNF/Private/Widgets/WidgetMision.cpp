// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WidgetMision.h"
#include "Components/Button.h"
#include "Actors/Characters/NPCMision.h"
#include "GameFramework/PlayerController.h"

void UWidgetMision::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (Button_Entregar)
	{
		Button_Entregar->OnClicked.AddDynamic(this, &UWidgetMision::OnButtonEntregarClicked);
	}
}

void UWidgetMision::OnButtonEntregarClicked()
{
	if (NPCVinculado)
	{
		APawn* JugadorPawn = GetOwningPlayerPawn();
		
		NPCVinculado->EntregarMateriales(JugadorPawn);
		
		RemoveFromParent();
		
		APlayerController* PC = GetOwningPlayer();
		if (PC)
		{
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(false);
		}
	}
}
