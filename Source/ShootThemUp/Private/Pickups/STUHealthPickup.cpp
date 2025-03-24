// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Pickups/STUHealthPickup.h"

#include "Player/Components/STUHealthComponent.h"
#include "STUUtilities.h"

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    if (!PlayerPawn)
    {
        return false;
    }

    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
    if (HealthComponent)
    {
        return HealthComponent->TryToAddHealth(HealthAmount);
    }
    return false;
}
