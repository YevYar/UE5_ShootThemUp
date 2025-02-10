// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "STUCoreTypes.generated.h"

// ----- HEALTH
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHealthChangedSignature, float, Health, bool, IsCausedByDamage, float,
                                               LastDamage);

// ----- WEAPON
class ASTUBaseWeapon;

DECLARE_MULTICAST_DELEGATE_OneParam(FReloadRequiredSignature, ASTUBaseWeapon*);

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

USTRUCT(BlueprintType) struct FWeaponUIData
{
        GENERATED_USTRUCT_BODY()

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponUI")
        UTexture2D* WeaponIcon = nullptr;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponUI")
        UTexture2D* WeaponCrosshair = nullptr;
};

// ----- VFX
class UMaterialInterface;
class UNiagaraSystem;

USTRUCT(BlueprintType) struct FDecalData
{
        GENERATED_USTRUCT_BODY()

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        UMaterialInterface* Material = nullptr;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        FVector Size = FVector{15.0f};
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        float LifeTime = 5.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        float FadeOutTime = 2.0f;
};

USTRUCT(BlueprintType) struct FImpactData
{
        GENERATED_USTRUCT_BODY()

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        UNiagaraSystem* Effect = nullptr;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        FDecalData DecalData;
};
