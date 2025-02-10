// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "STUCoreTypes.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUWeaponVFXComponent.generated.h"

class UPhysicalMaterial;

UCLASS() class SHOOTTHEMUP_API USTUWeaponVFXComponent : public UActorComponent
{
        GENERATED_BODY()

    public:
        USTUWeaponVFXComponent();

        void PlayImpactVFX(const FHitResult& HitResult);

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        FImpactData DefaultImpactData;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;
};
