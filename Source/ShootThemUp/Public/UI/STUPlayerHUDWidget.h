// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUPlayerHUDWidget.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
        GENERATED_BODY()

    public:
        bool Initialize() override;

        UFUNCTION(BlueprintCallable, Category = "Health")
        float GetHealthPercent() const;
        UFUNCTION(BlueprintImplementableEvent, Category = "Health")
        void OnTakeDamage();

        UFUNCTION(BlueprintCallable, Category = "PlayerStatus")
        bool IsPlayerAlive() const;
        UFUNCTION(BlueprintCallable, Category = "PlayerStatus")
        bool IsPlayerSpectating() const;

        UFUNCTION(BlueprintCallable, Category = "Weapon")
        bool GetCurrentWeaponAmmoData(FAmmoData& WeaponAmmoData) const;
        UFUNCTION(BlueprintCallable, Category = "Weapon")
        bool GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const;

    private:
        UFUNCTION()
        void OnHealthChanged(float NewHealth, bool IsCausedByDamage, float LastDamage);

        void OnNewPawn(APawn* NewPawn);
};
