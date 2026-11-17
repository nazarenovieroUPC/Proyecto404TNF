#include "Widgets/OverheadHealthWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UOverheadHealthWidget::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (PB_AllyHealth && MaxHealth > 0.0f)
	{
		const float Percent = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
		PB_AllyHealth->SetPercent(Percent);
	}
}
void UOverheadHealthWidget::UpdateRoleText(const FText& RoleText)
{
	if (Txt_AllyRole)
	{
		Txt_AllyRole->SetText(RoleText);
		Txt_AllyRole->SetVisibility(RoleText.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
}
void UOverheadHealthWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PB_AllyHealth)
	{
		const float Current = PB_AllyHealth->GetPercent();
		if (!FMath::IsNearlyEqual(Current, TargetPercent, 0.001f))
		{
			const float NewPercent = FMath::FInterpTo(Current, TargetPercent, InDeltaTime, 12.0f);
			PB_AllyHealth->SetPercent(NewPercent);
		}
	}
}