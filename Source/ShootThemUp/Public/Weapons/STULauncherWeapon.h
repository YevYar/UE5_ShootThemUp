// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "Weapons/STUBaseWeapon.h"

#include "STULauncherWeapon.generated.h"

class ASTULauncherProjectile;

UCLASS() class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
        GENERATED_BODY()

    public:
        void StartFire() override;
        void StopFire() override;

    protected:
        virtual void MakeShot();

        FVector GetTraceDirection(const FVector& ViewPointForwardVector) const override;

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        TSubclassOf<ASTULauncherProjectile> ProjectileType;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings",
                  meta = (ClampMin = "0.0", ToolTip = "In Degrees."))
        float ShootingSpreadConeAngle = 12.0f;
};
