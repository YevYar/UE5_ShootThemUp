// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUHealthBarWidget.h"

#include "Components/ProgressBar.h"

void USTUHealthBarWidget::SetHealthPercent(float HealthPercent)
{
    if (HealthProgressBar && HealthPercent >= 0.0 && HealthPercent <= 1.0)
    {
        const auto ProgressBarVisibility =
          (HealthPercent > HealthPercentVisibilityThreshold || FMath::IsNearlyZero(HealthPercent))
            ? ESlateVisibility::Hidden
            : ESlateVisibility::Visible;
        HealthProgressBar->SetVisibility(ProgressBarVisibility);

        const auto ProgressBarColor = HealthPercent > PercentColorThreshold ? GoodColor : BadColor;
        HealthProgressBar->SetFillColorAndOpacity(ProgressBarColor);

        HealthProgressBar->SetPercent(HealthPercent);
    }
}
