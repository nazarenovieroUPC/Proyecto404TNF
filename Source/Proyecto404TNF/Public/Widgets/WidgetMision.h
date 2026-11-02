#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMision.generated.h"

class UButton;
class UWidgetSwitcher;
class ANPCMision;

UCLASS()
class PROYECTO404TNF_API UWidgetMision : public UUserWidget
{
	GENERATED_BODY()
    
public:
	virtual void NativeOnInitialized() override;
    
	UPROPERTY(BlueprintReadWrite, Category = "NPC")
	TObjectPtr<ANPCMision> NPCVinculado;
    
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> SwitcherPaneles;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_PestanaMision;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_PestanaTienda;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Entregar;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Aceptar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Cerrar;
	
	UFUNCTION()
	void MostrarVistaMision();

	UFUNCTION()
	void MostrarVistaTienda();
	
	UFUNCTION()
	void OnButtonEntregarClicked();
    
	UFUNCTION()
	void OnButtonAceptarClicked();

	UFUNCTION()
	void OnButtonCerrarClicked();
};