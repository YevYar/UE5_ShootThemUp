// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "Weapons/STUBaseWeapon.h"

#include "STURifleWeapon.generated.h"

UCLASS() class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
        GENERATED_BODY()

    public:
        void StartFire() override;
        void StopFire() override;

    protected:
        virtual void MakeShot();

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSettings", meta = (ToolTip = "In Radians. The default is 9 degrees."))
        float ShootingSpreadConeAngle = 0.157;

        FTimerHandle BurstShootingTimer;
        float        TimeFromFireStart = 0.0f;
};
