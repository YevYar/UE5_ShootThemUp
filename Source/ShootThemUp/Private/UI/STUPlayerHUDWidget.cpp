// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUPlayerHUDWidget.h"

#include "Player/Components/STUHealthComponent.h"
#include "Weapons/Components/STUWeaponComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = GetHealthComponent();
    return HealthComponent ? HealthComponent->GetHealthPercent() : 0.0f;
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = GetHealthComponent();
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& WeaponAmmoData) const
{
    const auto WeaponComponent = GetWeaponComponent();
    return WeaponComponent && WeaponComponent->GetCurrentWeaponAmmoData(WeaponAmmoData);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    const auto WeaponComponent = GetWeaponComponent();
    return WeaponComponent && WeaponComponent->GetCurrentWeaponUIData(WeaponUIData);
}

USTUHealthComponent* USTUPlayerHUDWidget::GetHealthComponent() const
{
    const auto PlayerPawn = GetOwningPlayerPawn();
    if (!PlayerPawn)
    {
        return nullptr;
    }

    return Cast<USTUHealthComponent>(PlayerPawn->FindComponentByClass(USTUHealthComponent::StaticClass()));
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
    const auto PlayerPawn = GetOwningPlayerPawn();
    if (!PlayerPawn)
    {
        return nullptr;
    }

    return Cast<USTUWeaponComponent>(PlayerPawn->FindComponentByClass(USTUWeaponComponent::StaticClass()));
}
