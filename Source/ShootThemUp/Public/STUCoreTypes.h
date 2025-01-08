// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "STUCoreTypes.generated.h"

// ----- HEALTH
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHealthChangedSignature, float, Health, bool, IsCausedByDamage, float,
                                               LastDamage);

// ----- WEAPON
DECLARE_MULTICAST_DELEGATE(FReloadRequiredSignature);

class ASTUBaseWeapon;

USTRUCT(BlueprintType) struct FAmmoData
{
        GENERATED_USTRUCT_BODY()

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        int32 BulletsAmount = 0;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings",
                  meta = (EditCondition = "!IsClipsInfinite"))
        int32 ClipsAmount = 0;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        bool IsClipsInfinite = false;
};

USTRUCT(BlueprintType) struct FWeaponSpawnData
{
        GENERATED_USTRUCT_BODY()

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        TSubclassOf<ASTUBaseWeapon> WeaponClass;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        UAnimMontage* ReloadAnimMontage = nullptr;
};
