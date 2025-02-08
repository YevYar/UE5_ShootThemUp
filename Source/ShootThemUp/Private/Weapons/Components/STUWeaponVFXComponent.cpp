// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/Components/STUWeaponVFXComponent.h"

#include "NiagaraFunctionLibrary.h"

USTUWeaponVFXComponent::USTUWeaponVFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponVFXComponent::PlayImpactVFX(const FHitResult& HitResult)
{
    if (GetWorld() && Effect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, HitResult.ImpactPoint,
                                                       HitResult.ImpactNormal.Rotation());
    }
}
