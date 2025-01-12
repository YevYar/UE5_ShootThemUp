// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUPlayerHUDWidget.generated.h"

class USTUWeaponComponent;

UCLASS() class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
        GENERATED_BODY()

    public:
        UFUNCTION(BlueprintCallable, Category = "Health")
        float GetHealthPercent() const;
        UFUNCTION(BlueprintCallable, Category = "Weapon")
        bool GetCurrentWeaponAmmoData(FAmmoData& WeaponAmmoData) const;
        UFUNCTION(BlueprintCallable, Category = "Weapon")
        bool GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const;

    private:
        USTUWeaponComponent* GetWeaponComponent() const;
};
