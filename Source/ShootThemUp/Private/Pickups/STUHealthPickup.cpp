// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Pickups/STUHealthPickup.h"

#include "STUUtilities.h"
#include "Player/Components/STUHealthComponent.h"

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    if (!PlayerPawn)
    {
        return false;
    }

    const auto HealthComponent = GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
    if (HealthComponent)
    {
        return HealthComponent->TryToAddHealth(HealthAmount);
    }
    return false;
}
