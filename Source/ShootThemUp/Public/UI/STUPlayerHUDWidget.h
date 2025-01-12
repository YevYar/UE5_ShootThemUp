// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUPlayerHUDWidget.generated.h"

class USTUHealthComponent;
class USTUWeaponComponent;

UCLASS() class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
        GENERATED_BODY()

    public:
        UFUNCTION(BlueprintCallable, Category = "Health")
        float GetHealthPercent() const;
        UFUNCTION(BlueprintCallable, Category = "Health")
        bool IsPlayerAlive() const;

        UFUNCTION(BlueprintCallable, Category = "Weapon")
        bool GetCurrentWeaponAmmoData(FAmmoData& WeaponAmmoData) const;
        UFUNCTION(BlueprintCallable, Category = "Weapon")
        bool GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const;

    private:
        USTUHealthComponent* GetHealthComponent() const;
        USTUWeaponComponent* GetWeaponComponent() const;
};
