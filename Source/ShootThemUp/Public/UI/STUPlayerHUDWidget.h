// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "UI/STUBaseWidget.h"

#include "STUPlayerHUDWidget.generated.h"

class UProgressBar;

UCLASS() class SHOOTTHEMUP_API USTUPlayerHUDWidget : public USTUBaseWidget
{
        GENERATED_BODY()

    public:
        UFUNCTION(BlueprintCallable, Category = "Weapon")
        FString GetFormattedBulletsText(int32 BulletsAmount) const;
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

    protected:
        void NativeOnInitialized() override;

    private:
        UFUNCTION()
        void OnHealthChanged(float NewHealth, bool IsCausedByDamage, float LastDamage);

        void OnNewPawn(APawn* NewPawn);
        void UpdateHealthBar();

    protected:
        UPROPERTY(meta = (BindWidgetAnim), Transient)
        UWidgetAnimation* DamageAnimation;
        UPROPERTY(meta = (BindWidget))
        UProgressBar* HealthProgressBar;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        FLinearColor HealthBadColor = FLinearColor::Red;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        FLinearColor HealthGoodColor = FLinearColor::Green;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        float PercentHealthColorThreshold = 0.3;
};
