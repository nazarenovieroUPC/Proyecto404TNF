#include "Widgets/WidgetMision.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/InventoryComponent.h"
#include "Actors/Characters/NPCMision.h"
#include "GameStates/MisionSystemState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Proyecto404TNF/Proyecto404TNFCharacter.h"

void UWidgetMision::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    if (Button_Entregar) Button_Entregar->OnClicked.AddDynamic(this, &UWidgetMision::OnButtonEntregarClicked);
    if (Button_Aceptar) Button_Aceptar->OnClicked.AddDynamic(this, &UWidgetMision::OnButtonAceptarClicked);
    if (Button_Cerrar) Button_Cerrar->OnClicked.AddDynamic(this, &UWidgetMision::OnButtonCerrarClicked);
    
    if (Btn_PestanaMision) Btn_PestanaMision->OnClicked.AddDynamic(this, &UWidgetMision::MostrarVistaMision);
    if (Btn_PestanaTienda) Btn_PestanaTienda->OnClicked.AddDynamic(this, &UWidgetMision::MostrarVistaTienda);
}

void UWidgetMision::NativeConstruct()
{
    Super::NativeConstruct();

    bEsperandoConfirmacionAceptar = false;
    ActualizarEstadoMisionUI();
}

void UWidgetMision::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    ActualizarEstadoMisionUI();
}

void UWidgetMision::ActualizarEstadoMisionUI()
{
    AMisionSystemState* GameStateMision = Cast<AMisionSystemState>(UGameplayStatics::GetGameState(this));
    if (!GameStateMision) return;
    
    if (GameStateMision->ItemsRecolectados >= GameStateMision->ItemsNecesarios)
    {
        if (Button_Aceptar) Button_Aceptar->SetIsEnabled(false);
        if (Txt_BtnAceptar) Txt_BtnAceptar->SetText(FText::FromString(TEXT("Misión Completada")));

        if (Button_Entregar) Button_Entregar->SetIsEnabled(false);
        if (Txt_BtnEntregar) Txt_BtnEntregar->SetText(FText::FromString(TEXT("¡Puente Reparado!")));

        if (Txt_EstadoMision)
        {
            Txt_EstadoMision->SetText(FText::FromString(TEXT("ESTADO: ¡Misión completada! El puente está construido.")));
            Txt_EstadoMision->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
        }
        return;
    }
    
    if (!GameStateMision->bMisionActiva && !bEsperandoConfirmacionAceptar)
    {
        if (Button_Aceptar) Button_Aceptar->SetIsEnabled(true);
        if (Txt_BtnAceptar) Txt_BtnAceptar->SetText(FText::FromString(TEXT("Aceptar Misión")));

        if (Button_Entregar) Button_Entregar->SetIsEnabled(false);
        if (Txt_BtnEntregar) Txt_BtnEntregar->SetText(FText::FromString(TEXT("Acepta primero")));

        if (Txt_EstadoMision)
        {
            Txt_EstadoMision->SetText(FText::FromString(TEXT("— Aceptá la misión para comenzar —")));
            Txt_EstadoMision->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.8f, 0.2f)));
        }
        return;
    }
    
    if (Button_Aceptar) Button_Aceptar->SetIsEnabled(false);
    if (Txt_BtnAceptar) Txt_BtnAceptar->SetText(FText::FromString(TEXT("Misión Aceptada ✓")));

    int32 MaderaEnMochila = 0;
    if (APawn* Pawn = GetOwningPlayerPawn())
    {
        if (UInventoryComponent* Inv = Pawn->FindComponentByClass<UInventoryComponent>())
        {
            const FText ItemBuscado = NPCVinculado ? NPCVinculado->NombreItemRequerido : FText::FromString(TEXT("Madera"));
            MaderaEnMochila = Inv->ObtenerCantidadTotalPorNombre(ItemBuscado);
        }
    }

    if (MaderaEnMochila > 0)
    {
        if (Button_Entregar) Button_Entregar->SetIsEnabled(true);
        if (Txt_BtnEntregar)
        {
            Txt_BtnEntregar->SetText(FText::FromString(FString::Printf(TEXT("Entregar Madera (%d)"), MaderaEnMochila)));
        }
    }
    else
    {
        if (Button_Entregar) Button_Entregar->SetIsEnabled(false);
        if (Txt_BtnEntregar) Txt_BtnEntregar->SetText(FText::FromString(TEXT("Sin madera en mochila")));
    }

    if (Txt_EstadoMision)
    {
        Txt_EstadoMision->SetText(FText::FromString(FString::Printf(
            TEXT("En curso — Progreso: %d / %d"),
            GameStateMision->ItemsRecolectados,
            GameStateMision->ItemsNecesarios
        )));
        Txt_EstadoMision->SetColorAndOpacity(FSlateColor(FLinearColor(0.2f, 0.9f, 1.0f)));
    }
}

void UWidgetMision::MostrarVistaMision()
{
    if (SwitcherPaneles)
    {
       SwitcherPaneles->SetActiveWidgetIndex(0); 
    }
}

void UWidgetMision::MostrarVistaTienda()
{
    if (SwitcherPaneles)
    {
       SwitcherPaneles->SetActiveWidgetIndex(1); 
    }
}

void UWidgetMision::OnButtonEntregarClicked()
{
    if (NPCVinculado)
    {
        if (AProyecto404TNFCharacter* JugadorChar = Cast<AProyecto404TNFCharacter>(GetOwningPlayerPawn()))
        {
            JugadorChar->Server_EntregarMaterialesEnNPC(NPCVinculado);
        }
    }
}

void UWidgetMision::OnButtonAceptarClicked()
{
    if (NPCVinculado && !bEsperandoConfirmacionAceptar)
    {
        bEsperandoConfirmacionAceptar = true;
        if (AProyecto404TNFCharacter* JugadorChar = Cast<AProyecto404TNFCharacter>(GetOwningPlayerPawn()))
        {
            JugadorChar->Server_AceptarMisionEnNPC(NPCVinculado);
        }
        ActualizarEstadoMisionUI();
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