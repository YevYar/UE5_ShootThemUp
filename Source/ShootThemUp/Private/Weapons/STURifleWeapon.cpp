// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/STURifleWeapon.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

void ASTURifleWeapon::StartFire()
{
    TimeFromFireStart = 0.0f;
    MakeShot();
    GetWorldTimerManager().SetTimer(BurstShootingTimer, this, &ASTURifleWeapon::MakeShot, ShootingInterval, true);
}

void ASTURifleWeapon::StopFire()
{
    if (BurstShootingTimer.IsValid())
    {
        TimeFromFireStart = 0.0f;
        GetWorldTimerManager().ClearTimer(BurstShootingTimer);
    }
}

void ASTURifleWeapon::MakeShot()
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

    const auto TraceStartLocation = ViewPointLocation;
    const auto TraceDirection   = FMath::VRandCone(ViewPointRotation.Vector(), FMath::Clamp(TimeFromFireStart / 4, 0.0f,
                                                                                            ShootingSpreadConeAngle / 2));
    const auto TraceEndLocation = TraceStartLocation + TraceDirection * ShootingDistance;

    const auto MuzzleTransform     = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const auto MuzzleForwardVector = MuzzleTransform.GetRotation().GetForwardVector();

    auto HitResult       = FHitResult{};
    auto CollisionParams = FCollisionQueryParams{};
    CollisionParams.AddIgnoredActor(Player);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation, ECC_Visibility,
                                             CollisionParams))
    {
        if (IsTargetAhead(MuzzleForwardVector, HitResult.ImpactPoint - MuzzleTransform.GetLocation()))
        {
            DrawDebugLine(GetWorld(), MuzzleTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f,
                          0.0f, 3.0f);
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 10.0f, FColor::Red, false, 2.0f, 0.0f, 3.0f);

            if (HitResult.GetActor())
            {
                ApplyDamageToTheHitActor(HitResult, MuzzleTransform.GetLocation());
            }
        }
    }
    else
    {
        if (IsTargetAhead(MuzzleForwardVector, TraceEndLocation - MuzzleTransform.GetLocation()))
        {
            DrawDebugLine(GetWorld(), MuzzleTransform.GetLocation(), TraceEndLocation, FColor::Green, false, 2.0f, 0.0f,
                          3.0f);
        }
    }

    TimeFromFireStart += ShootingInterval;
}
