// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Pickups/STUAmmoPickup.h"

#include "STUUtilities.h"
#include "Weapons/Components/STUWeaponComponent.h"

TSubclassOf<ASTUBaseWeapon> ASTUAmmoPickup::GetWeaponClass() const
{
    return WeaponClass;
}

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    if (!PlayerPawn)
    {
        return false;
    }

    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
    if (WeaponComponent)
    {
        return WeaponComponent->TryToAddAmmo(WeaponClass, ClipsAmount);
    }
    return false;
}
