// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/Components/STUWeaponVFXComponent.h"

#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
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
        auto ImpactData = DefaultImpactData;
        if (HitResult.PhysMaterial.IsValid() && ImpactDataMap.Contains(HitResult.PhysMaterial.Get()))
        {
            ImpactData = ImpactDataMap[HitResult.PhysMaterial.Get()];
        }

        // Niagara
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.Effect, HitResult.ImpactPoint,
                                                       HitResult.ImpactNormal.Rotation());

        // Decal
        auto SpawnedDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactData.DecalData.Material,
                                                                   ImpactData.DecalData.Size, HitResult.ImpactPoint,
                                                                   HitResult.ImpactNormal.Rotation());
        if (SpawnedDecal)
        {
            SpawnedDecal->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
        }
    }
}
