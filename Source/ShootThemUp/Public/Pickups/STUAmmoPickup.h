// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUAmmoPickup.generated.h"

UCLASS() class SHOOTTHEMUP_API ASTUAmmoPickup : public ASTUBasePickup
{
        GENERATED_BODY()

    protected:
        bool GivePickupTo(APawn* PlayerPawn) override;
};
