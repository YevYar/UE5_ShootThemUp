// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "Weapons/STUBaseWeapon.h"

#include "STULauncherWeapon.generated.h"

UCLASS() class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
        GENERATED_BODY()

    public:
        void StartFire() override;
        void StopFire() override;

    protected:
        virtual void MakeShot();

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSettings", meta = (ToolTip = "In Degrees."))
        float ShootingSpreadConeAngle = 12.0f;
};
