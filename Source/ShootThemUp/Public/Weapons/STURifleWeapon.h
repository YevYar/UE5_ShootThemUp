// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "Weapons/STUBaseWeapon.h"

#include "STURifleWeapon.generated.h"

class UAudioComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USTUWeaponVFXComponent;

UCLASS() class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
        GENERATED_BODY()

    public:
        ASTURifleWeapon();

        void StartFire() override;
        void StopFire() override;
        void Zoom(bool Enabled) override;

        EWeaponType GetWeaponType() const noexcept override;

    protected:
        void BeginPlay() override;

        float   CalculateDamage(float DistanceFromMuzzle, float DistanceFromTraceStartToMuzzle) const override;
        FVector GetTraceDirection(const FVector& ViewPointForwardVector) const override;

        virtual bool MakeShot();

        void InitMuzzleEffects();
        void MakeShotTimerSlot();
        void SetMuzzleEffectsActive(bool IsActive);
        void SpawnTraceEffect(const FVector& TraceStart, const FVector& TraceEnd);

    private:
        void AttachMuzzleSoundComponentToMuzzle();
        void ReattachMuzzleSound();

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
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        float ZoomFOVAngle = 50.0f;

        UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "VFX")
        UNiagaraComponent* MuzzleEffectComponent;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        UNiagaraSystem* TraceTargetEffect;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        FName TraceTargetName = "TraceTarget";
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        USTUWeaponVFXComponent* VFXComponent;

        UPROPERTY()
        UAudioComponent* FireAudioComponent;

        FTimerHandle BurstShootingTimer;
        float        TimeFromFireStart = 0.0f;

    private:
        float DefaultFOVAngle = 90.0f;
};
