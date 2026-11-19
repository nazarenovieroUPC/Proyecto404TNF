#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMision.generated.h"

class UButton;
class UTextBlock;
class UWidgetSwitcher;
class ANPCMision;

UCLASS()
class PROYECTO404TNF_API UWidgetMision : public UUserWidget
{
	GENERATED_BODY()
    
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
	UPROPERTY(BlueprintReadWrite, Category = "NPC")
	TObjectPtr<ANPCMision> NPCVinculado;

	UFUNCTION(BlueprintCallable, Category = "Mision")
	void ActualizarEstadoMisionUI();
    
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
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_BtnAceptar;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_BtnEntregar;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_EstadoMision;
    
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

private:
	bool bEsperandoConfirmacionAceptar = false;
};