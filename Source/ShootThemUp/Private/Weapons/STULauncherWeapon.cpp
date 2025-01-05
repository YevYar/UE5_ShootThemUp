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

    const auto MuzzleTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);

    const auto ProjactileTranform = FTransform{FRotator::ZeroRotator, MuzzleTransform.GetLocation()};
    auto SpawnedProjectile = GetWorld()->SpawnActorDeferred<ASTULauncherProjectile>(ProjectileType, ProjactileTranform);

    if (SpawnedProjectile)
    {
        const auto ShootingDirection = (TraceEndLocation - MuzzleTransform.GetLocation()).GetSafeNormal();
        SpawnedProjectile->SetShotDirection(ShootingDirection);
        SpawnedProjectile->SetOwner(GetOwner());

        const auto Character = Cast<ASTUBaseCharacter>(GetOwner());
        if (Character)
        {
            Character->MoveIgnoreActorAdd(SpawnedProjectile);
        }

        SpawnedProjectile->FinishSpawning(ProjactileTranform);

        DrawDebugLine(GetWorld(), MuzzleTransform.GetLocation(), TraceEndLocation, FColor::Red, false, 2.0f, 0.0f,
                      3.0f);
    }

    DecreaseBullets();
}

FVector ASTULauncherWeapon::GetTraceDirection(const FVector& ViewPointForwardVector) const
{
    const auto ShootingConeHalfRad = FMath::DegreesToRadians(ShootingSpreadConeAngle) / 2.0f;
    return FMath::VRandCone(ViewPointForwardVector, FMath::RandRange(0.0f, ShootingConeHalfRad));
}
