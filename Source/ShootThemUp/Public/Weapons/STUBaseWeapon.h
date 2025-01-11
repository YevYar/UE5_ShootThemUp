// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "STUCoreTypes.h"

#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS() class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
        GENERATED_BODY()

    public:
        ASTUBaseWeapon();

        UFUNCTION(BlueprintCallable, Category = "Actions")
        virtual bool CanReload() const;
        /**
         * \return false if there is no clip to change the current clip, true otherwise.
         */
        UFUNCTION(BlueprintCallable, Category = "Actions")
        bool ChangeClip();
        UFUNCTION(BlueprintCallable, Category = "Actions")
        virtual void StartFire();
        UFUNCTION(BlueprintCallable, Category = "Actions")
        virtual void StopFire();

        FWeaponUIData GetWeaponUIData() const noexcept;
        bool          IsAmmoEmpty() const;

    protected:
        static FVector GetShotDirection(const FVector_NetQuantize& ImpactPoint, const FVector& MuzzleLocation);
        static bool    IsTargetAhead(const FVector& MuzzleForwardVector, const FVector& Target);

        bool GetPlayerAndController(ACharacter*& OutPlayer, AController*& OutController) const;

        // --------------- AMMO METHODS ---------------
        void DecreaseBullets();
        bool IsClipEmpty() const;
        void LogAmmo() const;

        void BeginPlay() override;

        virtual void    ApplyDamageToTheHitActor(const FHitResult& HitResult, const FVector& MuzzleLocation) const;
        virtual float   CalculateDamage(float DistanceFromMuzzle, float DistanceFromTraceStartToMuzzle) const;
        virtual bool    GetTraceData(FVector& OutTraceStartLocation, FVector& OutTraceEndLocation) const;
        virtual FVector GetTraceDirection(const FVector& ViewPointForwardVector) const;

    public:
        FReloadRequiredSignature ReloadRequired;

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USkeletalMeshComponent* WeaponMesh;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        FAmmoData DefaultAmmo = FAmmoData{20, 5, false};
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        FName MuzzleSocketName = "MuzzleSocket";
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        float ShootingDistance = 1500.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        FWeaponUIData WeaponUIData;

    private:
        FAmmoData CurrentAmmo = FAmmoData{20, 5, false};
};
