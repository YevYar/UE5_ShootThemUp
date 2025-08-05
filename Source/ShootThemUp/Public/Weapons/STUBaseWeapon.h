// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "STUCoreTypes.h"

#include "STUBaseWeapon.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USkeletalMeshComponent;
class USoundCue;

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
        UFUNCTION(BlueprintCallable, Category = "Actions")
        virtual void StopFireAndZoom();
        UFUNCTION(BlueprintCallable, Category = "Actions")
        virtual void Zoom(bool Enabled);

        virtual bool TryToAddAmmo(int32 ClipsAmount);

        FVector             GetMuzzleLocation() const;
        FRotator            GetMuzzleRotation() const;
        FTransform          GetMuzzleTransform() const;
        FAmmoData           GetWeaponAmmoData() const noexcept;
        virtual EWeaponType GetWeaponType() const noexcept;
        FWeaponUIData       GetWeaponUIData() const noexcept;
        bool                IsAmmoEmpty() const;
        bool                IsAmmoFull() const;
        void                PlayNoAmmoSound() const;

    protected:
        static FVector GetShotDirection(const FVector_NetQuantize& ImpactPoint, const FVector& MuzzleLocation);
        static bool    IsTargetAhead(const FVector& MuzzleForwardVector, const FVector& Target);

        UFUNCTION()
        void OnMuzzleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult);
        UFUNCTION()
        void OnMuzzleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

        bool               GetPlayerAndController(ACharacter*& OutPlayer, AController*& OutController) const;
        UNiagaraComponent* SpawnMuzzleEffect();
        UAudioComponent*   SpawnMuzzleSound();

        // --------------- AMMO METHODS ---------------
        void DecreaseBullets();
        bool IsClipEmpty() const;
        void LogAmmo() const;

        void BeginPlay() override;

        virtual void    ApplyDamageToTheHitActor(const FHitResult& HitResult, const FVector& MuzzleLocation) const;
        virtual float   CalculateDamage(float DistanceFromMuzzle, float DistanceFromTraceStartToMuzzle) const;
        virtual bool    GetTraceData(FVector& OutTraceStartLocation, FVector& OutTraceEndLocation) const;
        virtual FVector GetTraceDirection(const FVector& ViewPointForwardVector) const;

        void GetPlayerViewPoint(FVector& ViewPointLocation, FRotator& ViewPointRotation) const;

    public:
        FReloadRequiredSignature ReloadRequired;

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UCapsuleComponent* MuzzleCollisionComponent;
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USkeletalMeshComponent* WeaponMesh;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
        USoundCue* FireSound;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
        USoundCue* NoAmmoSound;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        UNiagaraSystem* MuzzleEffect;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        FAmmoData DefaultAmmo = FAmmoData{20, 5, false};
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        FName MuzzleSocketName = "MuzzleSocket";
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        float ShootingDistance = 1500.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        FWeaponUIData WeaponUIData;

        bool bIsMuzzleOverlaped = false;

    private:
        FAmmoData CurrentAmmo = FAmmoData{20, 5, false};
};
