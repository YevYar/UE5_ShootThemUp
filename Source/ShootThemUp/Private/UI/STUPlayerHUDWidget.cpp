// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUPlayerHUDWidget.h"

#include "Components/ProgressBar.h"

#include "Player/Components/STUHealthComponent.h"
#include "STUUtilities.h"
#include "Weapons/Components/STUWeaponComponent.h"

FString USTUPlayerHUDWidget::GetFormattedBulletsText(int32 BulletsAmount) const
{
    const auto MaxLen       = int32{2};
    const auto PrefixSymbol = '0';

    const auto BulletsStr      = FString::FromInt(BulletsAmount);
    const auto SymbolsNumToAdd = MaxLen - BulletsStr.Len();

    return SymbolsNumToAdd > 0 ? FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletsStr) : BulletsStr;
}

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

    UpdateHealthBar();
}

void USTUPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn);
    if (HealthComponent && !HealthComponent->HealthChanged.Contains(this, "OnHealthChanged"))
    {
        HealthComponent->HealthChanged.AddDynamic(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }

    UpdateHealthBar();
}

void USTUPlayerHUDWidget::UpdateHealthBar()
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentHealthColorThreshold ? HealthGoodColor
                                                                                                   : HealthBadColor);
    }
}
