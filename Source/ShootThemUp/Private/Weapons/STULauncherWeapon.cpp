// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/STULauncherWeapon.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::StopFire()
{
}

void ASTULauncherWeapon::MakeShot()
{
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

    const auto MuzzleTransform     = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const auto MuzzleForwardVector = MuzzleTransform.GetRotation().GetForwardVector();

    auto HitResult       = FHitResult{};
    auto CollisionParams = FCollisionQueryParams{};
    CollisionParams.AddIgnoredActor(Player);
}
