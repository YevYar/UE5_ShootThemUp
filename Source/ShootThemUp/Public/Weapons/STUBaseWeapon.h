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
        virtual void StartFire();
        UFUNCTION(BlueprintCallable, Category = "Actions")
        virtual void StopFire();

    protected:
        static FVector GetShotDirection(const FVector_NetQuantize& ImpactPoint, const FVector& MuzzleLocation);
        static bool    IsTargetAhead(const FVector& MuzzleForwardVector, const FVector& Target);

        bool GetPlayerAndController(ACharacter*& OutPlayer, AController*& OutController) const;

        void BeginPlay() override;

        virtual void    ApplyDamageToTheHitActor(const FHitResult& HitResult, const FVector& MuzzleLocation) const;
        virtual float   CalculateDamage(float DistanceFromMuzzle, float DistanceFromTraceStartToMuzzle) const;
        virtual bool    GetTraceData(FVector& OutTraceStartLocation, FVector& OutTraceEndLocation) const;
        virtual FVector GetTraceDirection(const FVector& ViewPointForwardVector) const;

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USkeletalMeshComponent* WeaponMesh;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        FName MuzzleSocketName = "MuzzleSocket";
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        float ShootingDistance = 1500.0f;
};
