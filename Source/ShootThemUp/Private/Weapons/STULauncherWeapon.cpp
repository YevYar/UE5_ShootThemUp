// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/STULauncherWeapon.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

#include "Player/STUBaseCharacter.h"
#include "Weapons/STULauncherProjectile.h"

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::StopFire()
{
}

EWeaponType ASTULauncherWeapon::GetWeaponType() const noexcept
{
    return EWeaponType::EWT_Launcher;
}

void ASTULauncherWeapon::MakeShot()
{
    if (IsAmmoEmpty())
    {
        return;
    }

    auto TraceStartLocation = FVector{};
    auto TraceEndLocation   = FVector{};
    if (!GetTraceData(TraceStartLocation, TraceEndLocation))
    {
        return;
    }

    UE_LOG(LogTemp, Error, TEXT("Launcher shot!"));

    const auto MuzzleLocation = GetMuzzleLocation();

    const auto ProjactileTranform = FTransform{FRotator::ZeroRotator, MuzzleLocation};
    auto SpawnedProjectile = GetWorld()->SpawnActorDeferred<ASTULauncherProjectile>(ProjectileType, ProjactileTranform);

    if (SpawnedProjectile)
    {
        const auto ShootingDirection = (TraceEndLocation - MuzzleLocation).GetSafeNormal();
        SpawnedProjectile->SetShotDirection(ShootingDirection);
        SpawnedProjectile->SetOwner(GetOwner());

        const auto Character = Cast<ASTUBaseCharacter>(GetOwner());
        if (Character)
        {
            Character->MoveIgnoreActorAdd(SpawnedProjectile);
        }

        SpawnedProjectile->FinishSpawning(ProjactileTranform);

        // DrawDebugLine(GetWorld(), MuzzleLocation, TraceEndLocation, FColor::Red, false, 2.0f, 0.0f, 3.0f);
    }

    DecreaseBullets();
    SpawnMuzzleEffect();
}

FVector ASTULauncherWeapon::GetTraceDirection(const FVector& ViewPointForwardVector) const
{
    const auto ShootingConeHalfRad = FMath::DegreesToRadians(ShootingSpreadConeAngle) / 2.0f;
    return FMath::VRandCone(ViewPointForwardVector, FMath::RandRange(0.0f, ShootingConeHalfRad));
}
