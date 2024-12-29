// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/STULauncherWeapon.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

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
    UE_LOG(LogTemp, Error, TEXT("Launcher shot!"));

    const auto Player = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Player)
    {
        return;
    }

    const auto Controller = Player->GetController();
    if (!Controller)
    {
        return;
    }

    auto ViewPointLocation = FVector{};
    auto ViewPointRotation = FRotator{};
    Controller->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

    const auto TraceStartLocation  = ViewPointLocation;
    const auto ShootingConeHalfRad = FMath::DegreesToRadians(ShootingSpreadConeAngle) / 2.0f;
    const auto TraceDirection =
      FMath::VRandCone(ViewPointRotation.Vector(), FMath::RandRange(0.0f, ShootingConeHalfRad));
    const auto TraceEndLocation = TraceStartLocation + TraceDirection * ShootingDistance;
    const auto MuzzleTransform  = WeaponMesh->GetSocketTransform(MuzzleSocketName);

    const auto ProjactileTranform = FTransform{FRotator::ZeroRotator, MuzzleTransform.GetLocation()};
    auto SpawnedProjectile = GetWorld()->SpawnActorDeferred<ASTULauncherProjectile>(ProjectileType, ProjactileTranform);

    if (SpawnedProjectile)
    {
        const auto ShootingDirection = (TraceEndLocation - MuzzleTransform.GetLocation()).GetSafeNormal();
        SpawnedProjectile->SetShotDirection(ShootingDirection);
        SpawnedProjectile->SetOwner(GetOwner());
        SpawnedProjectile->FinishSpawning(ProjactileTranform);

        DrawDebugLine(GetWorld(), MuzzleTransform.GetLocation(), TraceEndLocation, FColor::Red, false, 2.0f, 0.0f,
                      3.0f);
    }
}
