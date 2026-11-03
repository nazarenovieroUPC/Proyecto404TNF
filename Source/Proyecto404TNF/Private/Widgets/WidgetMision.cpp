#include "Widgets/WidgetMision.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Actors/Characters/NPCMision.h"
#include "GameFramework/PlayerController.h"
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
        OnButtonCerrarClicked(); 
    }
}

void UWidgetMision::OnButtonAceptarClicked()
{
    if (NPCVinculado)
    {
        if (AProyecto404TNFCharacter* JugadorChar = Cast<AProyecto404TNFCharacter>(GetOwningPlayerPawn()))
        {
            JugadorChar->Server_AceptarMisionEnNPC(NPCVinculado);
        }
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