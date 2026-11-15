#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadHealthWidget.generated.h"

class UProgressBar;

UCLASS()
class PROYECTO404TNF_API UOverheadHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_AllyHealth;
};