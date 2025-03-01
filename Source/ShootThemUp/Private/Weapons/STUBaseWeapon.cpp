// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/STUBaseWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.BulletsAmount < DefaultAmmo.BulletsAmount
           && (CurrentAmmo.IsClipsInfinite || CurrentAmmo.ClipsAmount > 0);
}

bool ASTUBaseWeapon::ChangeClip()
{
    StopFire();

    if (!CanReload())
    {
        return false;
    }

    if (!CurrentAmmo.IsClipsInfinite)
    {
        --CurrentAmmo.ClipsAmount;
    }

    CurrentAmmo.BulletsAmount = DefaultAmmo.BulletsAmount;

    UE_LOG(LogBaseWeapon, Display, TEXT("---- CHANGE CLIP ----"));

    return true;
}

void ASTUBaseWeapon::StartFire()
{
    UE_LOG(LogBaseWeapon, All, TEXT("Start Fire!"));
}

void ASTUBaseWeapon::StopFire()
{
    UE_LOG(LogBaseWeapon, All, TEXT("Stop Fire!"));
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (ClipsAmount < 0 || IsAmmoFull())
    {
        return false;
    }

    if (IsAmmoEmpty())
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty."));

        CurrentAmmo.ClipsAmount = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.ClipsAmount);
        ReloadRequired.Broadcast(this);
    }
    else if (CurrentAmmo.ClipsAmount < DefaultAmmo.ClipsAmount)
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added."));

        const auto NextClipsAmount = CurrentAmmo.ClipsAmount + ClipsAmount;

        CurrentAmmo.ClipsAmount = DefaultAmmo.ClipsAmount - NextClipsAmount >= 0 ? NextClipsAmount
                                                                                 : DefaultAmmo.ClipsAmount + 1;

        if (CurrentAmmo.BulletsAmount != DefaultAmmo.BulletsAmount)
        {
            UE_LOG(LogBaseWeapon, Display, TEXT("Reload current clip."));

            ReloadRequired.Broadcast(this);
        }
        else if (CurrentAmmo.ClipsAmount - DefaultAmmo.ClipsAmount == 1)
        {
            CurrentAmmo.ClipsAmount--;
        }
    }
    else if (CurrentAmmo.BulletsAmount != DefaultAmmo.BulletsAmount)
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Clips were full, reload current clip."));

        CurrentAmmo.ClipsAmount++;
        ReloadRequired.Broadcast(this);
    }

    return true;
}

FAmmoData ASTUBaseWeapon::GetWeaponAmmoData() const noexcept
{
    return CurrentAmmo;
}

FWeaponUIData ASTUBaseWeapon::GetWeaponUIData() const noexcept
{
    return WeaponUIData;
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.IsClipsInfinite && CurrentAmmo.ClipsAmount == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return (CurrentAmmo.IsClipsInfinite || CurrentAmmo.ClipsAmount == DefaultAmmo.ClipsAmount)
           && CurrentAmmo.BulletsAmount == DefaultAmmo.BulletsAmount;
}

FVector ASTUBaseWeapon::GetShotDirection(const FVector_NetQuantize& ImpactPoint, const FVector& MuzzleLocation)
{
    return (ImpactPoint - MuzzleLocation).GetSafeNormal();
}

bool ASTUBaseWeapon::IsTargetAhead(const FVector& MuzzleForwardVector, const FVector& Target)
{
    return FVector::DotProduct(MuzzleForwardVector, Target) > 0.0f;
}

bool ASTUBaseWeapon::GetPlayerAndController(ACharacter*& OutPlayer, AController*& OutController) const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Player)
    {
        return false;
    }

    const auto Controller = Player->GetController();
    if (!Controller)
    {
        return false;
    }

    OutPlayer     = Player;
    OutController = Controller;
    return true;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleEffect()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleEffect, WeaponMesh, MuzzleSocketName, FVector::ZeroVector,
                                                        FRotator::ZeroRotator,
                                                        EAttachLocation::SnapToTargetIncludingScale, true);
}

void ASTUBaseWeapon::DecreaseBullets()
{
    --CurrentAmmo.BulletsAmount;
    // LogAmmo();

    if (IsClipEmpty())
    {
        StopFire();
        ReloadRequired.Broadcast(this);
    }
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.BulletsAmount == 0;
}

void ASTUBaseWeapon::LogAmmo() const
{
    const auto AmmoMessage =
      FString::Printf(TEXT("Ammo: %d/%s"), CurrentAmmo.BulletsAmount,
                      *(CurrentAmmo.IsClipsInfinite ? "Infinite" : FString::FromInt(CurrentAmmo.ClipsAmount)));
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoMessage);
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    checkf(DefaultAmmo.BulletsAmount > 0, TEXT("The bullets amount must be above 0!"));
    checkf(DefaultAmmo.ClipsAmount > 0, TEXT("The clips amount must be above 0!"));
    checkf(ShootingDistance > 0, TEXT("The shooting distance must be above 0!"));

    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::ApplyDamageToTheHitActor(const FHitResult& HitResult, const FVector& MuzzleLocation) const
{
    ACharacter*  Player     = nullptr;
    AController* Controller = nullptr;
    if (!GetPlayerAndController(Player, Controller))
    {
        return;
    }

    if (HitResult.GetActor())
    {
        const auto DamageAmount  = CalculateDamage((HitResult.ImpactPoint - MuzzleLocation).Length(),
                                                   (HitResult.TraceStart - MuzzleLocation).Length());
        const auto ShotDirection = GetShotDirection(HitResult.ImpactPoint, MuzzleLocation);
        const auto PointDamageEvent =
          FPointDamageEvent{DamageAmount, HitResult, ShotDirection, UDamageType::StaticClass()};
        HitResult.GetActor()->TakeDamage(DamageAmount, PointDamageEvent, Controller, Player);

        UE_LOG(LogBaseWeapon, All, TEXT("Fire!"));
    }
}

float ASTUBaseWeapon::CalculateDamage(float DistanceFromMuzzle, float DistanceFromTraceStartToMuzzle) const
{
    return 0.0f;
}

bool ASTUBaseWeapon::GetTraceData(FVector& OutTraceStartLocation, FVector& OutTraceEndLocation) const
{
    ACharacter*  Player     = nullptr;
    AController* Controller = nullptr;
    if (!GetPlayerAndController(Player, Controller))
    {
        return false;
    }

    auto ViewPointLocation = FVector{};
    auto ViewPointRotation = FRotator{};
    GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

    OutTraceStartLocation     = ViewPointLocation;
    const auto TraceDirection = GetTraceDirection(ViewPointRotation.Vector());
    OutTraceEndLocation       = OutTraceStartLocation + TraceDirection * ShootingDistance;

    return true;
}

FVector ASTUBaseWeapon::GetTraceDirection(const FVector& ViewPointForwardVector) const
{
    return ViewPointForwardVector;
}

void ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewPointLocation, FRotator& ViewPointRotation) const
{
    ACharacter*  Player     = nullptr;
    AController* Controller = nullptr;
    if (!GetPlayerAndController(Player, Controller))
    {
        return;
    }

    if (Player->IsPlayerControlled())
    {
        Controller->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
    }
    else
    {
        ViewPointLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
        ViewPointRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }
}
