// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/Components/STUAIWeaponComponent.h"

#include "Algo/RandomShuffle.h"

#include "Weapons/STUBaseWeapon.h"

void USTUAIWeaponComponent::NextWeapon()
{
    if (CanEquipWeapon())
    {
        auto NextWeaponIdx = (CurrentWeaponIndex + 1) % SpawnedWeapons.Num();

        while (NextWeaponIdx != CurrentWeaponIndex)
        {
            if (NextWeaponIdx < SpawnedWeapons.Num() && SpawnedWeapons[NextWeaponIdx]
                && !SpawnedWeapons[NextWeaponIdx]->IsAmmoEmpty())
            {
                break;
            }

            NextWeaponIdx = (NextWeaponIdx + 1) % SpawnedWeapons.Num();
        }

        if (NextWeaponIdx != CurrentWeaponIndex)
        {
            EquipTheWeapon(NextWeaponIdx);
        }
    }
}

void USTUAIWeaponComponent::StartFire()
{
    if (CanFire())
    {
        CurrentWeapon->StartFire();
    }
    else if (!IsAnimMontageInProgress() && CurrentWeapon->IsAmmoEmpty())
    {
        NextWeapon();
    }
}

void USTUAIWeaponComponent::SpawnWeapons()
{
    Algo::RandomShuffle(WeaponDataToSpawn);
    Super::SpawnWeapons();
}
