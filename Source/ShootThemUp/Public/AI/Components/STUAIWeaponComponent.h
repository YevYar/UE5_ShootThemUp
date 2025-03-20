// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Components/STUWeaponComponent.h"

#include "STUAIWeaponComponent.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUAIWeaponComponent : public USTUWeaponComponent
{
        GENERATED_BODY()

    public:
        void NextWeapon() override;
        void StartFire() override;

    protected:
        void SpawnWeapons() override;
};
