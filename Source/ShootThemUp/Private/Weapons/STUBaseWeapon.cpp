// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/STUBaseWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"

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
    LogAmmo();

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

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.IsClipsInfinite && CurrentAmmo.ClipsAmount == 0 && IsClipEmpty();
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

void ASTUBaseWeapon::DecreaseBullets()
{
    --CurrentAmmo.BulletsAmount;
    LogAmmo();

    if (IsClipEmpty())
    {
        StopFire();
        ReloadRequired.Broadcast();
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
    Controller->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

    OutTraceStartLocation     = ViewPointLocation;
    const auto TraceDirection = GetTraceDirection(ViewPointRotation.Vector());
    OutTraceEndLocation       = OutTraceStartLocation + TraceDirection * ShootingDistance;

    return true;
}

FVector ASTUBaseWeapon::GetTraceDirection(const FVector& ViewPointForwardVector) const
{
    return ViewPointForwardVector;
}
