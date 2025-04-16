// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUPlayerHUDWidget.h"

#include "Player/Components/STUHealthComponent.h"
#include "STUUtilities.h"
#include "Weapons/Components/STUWeaponComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent ? HealthComponent->GetHealthPercent() : 0.0f;
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto PlayerController = GetOwningPlayer();
    return PlayerController && PlayerController->GetStateName() == NAME_Spectating;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& WeaponAmmoData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    return WeaponComponent && WeaponComponent->GetCurrentWeaponAmmoData(WeaponAmmoData);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    return WeaponComponent && WeaponComponent->GetCurrentWeaponUIData(WeaponUIData);
}

void USTUPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void USTUPlayerHUDWidget::OnHealthChanged(float NewHealth, bool IsCausedByDamage, float LastDamage)
{
    if (IsCausedByDamage)
    {
        OnTakeDamage();
    }
}

void USTUPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn);
    if (HealthComponent && !HealthComponent->HealthChanged.Contains(this, "OnHealthChanged"))
    {
        HealthComponent->HealthChanged.AddDynamic(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }
}
