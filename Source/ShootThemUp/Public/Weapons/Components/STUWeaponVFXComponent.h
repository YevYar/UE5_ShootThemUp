// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUWeaponVFXComponent.generated.h"

class UNiagaraSystem;
class UPhysicalMaterial;

UCLASS() class SHOOTTHEMUP_API USTUWeaponVFXComponent : public UActorComponent
{
        GENERATED_BODY()

    public:
        USTUWeaponVFXComponent();

        void PlayImpactVFX(const FHitResult& HitResult);

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        UNiagaraSystem* DefaultEffect;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        TMap<UPhysicalMaterial*, UNiagaraSystem*> MaterialEffects;
};
