// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "Weapons/STUBaseWeapon.h"

#include "STURifleWeapon.generated.h"

UCLASS() class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
        GENERATED_BODY()

    public:
        void Fire() override;

};
