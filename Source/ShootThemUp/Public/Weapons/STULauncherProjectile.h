// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "STULauncherProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS() class SHOOTTHEMUP_API ASTULauncherProjectile : public AActor
{
        GENERATED_BODY()

    public:
        ASTULauncherProjectile();

        UFUNCTION(BlueprintCallable, Category = "Actions")
        void SetShotDirection(const FVector& ShotDirection);

    protected:
        virtual void BeginPlay() override;

        UFUNCTION()
        virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& Hit);

    private:
        AController* GetController() const;

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UProjectileMovementComponent* MovementComponent;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        USphereComponent* SphereComponent;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectileSettings", meta = (ClampMin = "0.0"))
        float DamageRadius = 400.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectileSettings")
        bool DoFullDamage = false;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectileSettings", meta = (ClampMin = "0.0"))
        float MaxDamage = 100.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectileSettings", meta = (ClampMin = "0.0"))
        float MinDamage = 30.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectileSettings",
                  meta = (ClampMin = "0.0", ToolTip = "In Seconds."))
        float LifeSpan = 7.0f;

    private:
        FVector ShotDirection;
};
