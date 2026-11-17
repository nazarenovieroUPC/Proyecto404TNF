#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadHealthWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class PROYECTO404TNF_API UOverheadHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateRoleText(const FText& RoleText);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_AllyHealth;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_AllyRole;
	
	float TargetPercent = 1.0f;
};