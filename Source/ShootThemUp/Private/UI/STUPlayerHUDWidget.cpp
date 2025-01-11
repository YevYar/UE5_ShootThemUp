// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUPlayerHUDWidget.h"

#include "Player/Components/STUHealthComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const noexcept
{
    const auto PlayerPawn = GetOwningPlayerPawn();
    if (!PlayerPawn)
    {
        return 0.0f;
    }

    const auto HealthComponent =
      Cast<USTUHealthComponent>(PlayerPawn->FindComponentByClass(USTUHealthComponent::StaticClass()));
    return HealthComponent ? HealthComponent->GetHealthPercent() : 0.0f;
}
