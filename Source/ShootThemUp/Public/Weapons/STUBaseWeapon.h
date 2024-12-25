// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS() class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
        GENERATED_BODY()

    public:
        ASTUBaseWeapon();

        UFUNCTION(BlueprintCallable, Category = "Actions")
        virtual void Fire();

    protected:
        virtual void BeginPlay() override;

        virtual void  ApplyDamageToTheHitActor(const FHitResult& HitResult, const FVector& MuzzleLocation) const;
        virtual float CalculateDamage(float DistanceFromMuzzle, float DistanceFromTraceStartToMuzzle) const;
        FVector       GetShotDirection(const FVector_NetQuantize& ImpactPoint, const FVector& MuzzleLocation) const;
        bool          IsTargetAhead(const FVector& MuzzleForwardVector, const FVector& Target) const;

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USkeletalMeshComponent* WeaponMesh;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        float MaxDamage = 30.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        float MinDamage = 2.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        FName MuzzleSocketName = "MuzzleSocket";
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        float ShootingDistance = 1500.0f;
};
