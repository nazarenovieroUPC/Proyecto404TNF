// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WidgetMision.h"
#include "Components/Button.h"
#include "Actors/Characters/NPCMision.h"
#include "GameFramework/PlayerController.h"

void UWidgetMision::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (Button_Entregar) Button_Entregar->OnClicked.AddDynamic(this, &UWidgetMision::OnButtonEntregarClicked);
	if (Button_Aceptar) Button_Aceptar->OnClicked.AddDynamic(this, &UWidgetMision::OnButtonAceptarClicked);
	if (Button_Cerrar) Button_Cerrar->OnClicked.AddDynamic(this, &UWidgetMision::OnButtonCerrarClicked);
}

void UWidgetMision::OnButtonEntregarClicked()
{
	if (NPCVinculado)
	{
		APawn* JugadorPawn = GetOwningPlayerPawn();
		NPCVinculado->Server_EntregarMateriales(JugadorPawn); 
		OnButtonCerrarClicked(); 
	}
}

void UWidgetMision::OnButtonAceptarClicked()
{
	if (NPCVinculado)
	{
		NPCVinculado->Server_AceptarMision();
		OnButtonCerrarClicked();
	}
}

void UWidgetMision::OnButtonCerrarClicked()
{
	RemoveFromParent();
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
	}
}
