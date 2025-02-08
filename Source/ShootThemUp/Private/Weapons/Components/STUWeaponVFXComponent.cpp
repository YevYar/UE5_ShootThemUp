// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/Components/STUWeaponVFXComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

USTUWeaponVFXComponent::USTUWeaponVFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponVFXComponent::PlayImpactVFX(const FHitResult& HitResult)
{
    if (GetWorld())
    {
        auto Effect = DefaultEffect;
        if (HitResult.PhysMaterial.IsValid() && MaterialEffects.Contains(HitResult.PhysMaterial.Get()))
        {
            Effect = MaterialEffects[HitResult.PhysMaterial.Get()];
        }

        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, HitResult.ImpactPoint,
                                                       HitResult.ImpactNormal.Rotation());
    }
}
