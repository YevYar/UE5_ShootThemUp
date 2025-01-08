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

        float   CalculateDamage(float DistanceFromMuzzle, float DistanceFromTraceStartToMuzzle) const override;
        FVector GetTraceDirection(const FVector& ViewPointForwardVector) const override;

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings", meta = (ClampMin = "0.0"))
        float MaxDamage = 30.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings", meta = (ClampMin = "0.0"))
        float MinDamage = 15.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings", meta = (ClampMin = "0.0"))
        float ShootingInterval = 0.1f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings",
                  meta = (ClampMin = "0.0", ToolTip = "In Degrees."))
        float ShootingSpreadConeAngle = 9.0f;

        FTimerHandle BurstShootingTimer;
        float        TimeFromFireStart = 0.0f;
};
