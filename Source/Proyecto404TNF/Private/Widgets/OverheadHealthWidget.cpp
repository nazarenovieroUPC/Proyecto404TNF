#include "Widgets/OverheadHealthWidget.h"
#include "Components/ProgressBar.h"

void UOverheadHealthWidget::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (PB_AllyHealth && MaxHealth > 0.0f)
	{
		const float Percent = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
		PB_AllyHealth->SetPercent(Percent);
	}
}