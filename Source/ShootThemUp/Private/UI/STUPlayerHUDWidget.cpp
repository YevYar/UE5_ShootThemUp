// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUPlayerHUDWidget.h"

#include "Player/Components/STUHealthComponent.h"
#include "Weapons/Components/STUWeaponComponent.h"

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

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& WeaponUIData) const noexcept
{
    const auto PlayerPawn = GetOwningPlayerPawn();
    if (!PlayerPawn)
    {
        return false;
    }

    const auto WeaponComponent =
      Cast<USTUWeaponComponent>(PlayerPawn->FindComponentByClass(USTUWeaponComponent::StaticClass()));
    return WeaponComponent && WeaponComponent->GetWeaponUIData(WeaponUIData);
}
