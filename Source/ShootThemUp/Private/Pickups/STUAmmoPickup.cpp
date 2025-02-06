// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Pickups/STUAmmoPickup.h"

#include "STUUtilities.h"
#include "Weapons/Components/STUWeaponComponent.h"

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    if (!PlayerPawn)
    {
        return false;
    }

    const auto WeaponComponent = GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
    if (WeaponComponent)
    {
        return WeaponComponent->TryToAddAmmo(WeaponClass, ClipsAmount);
    }
    return false;
}
