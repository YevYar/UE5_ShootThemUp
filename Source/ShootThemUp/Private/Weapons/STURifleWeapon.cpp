// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/STURifleWeapon.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

void ASTURifleWeapon::Fire()
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
    const auto TraceDirection     = ViewPointRotation.Vector();
    const auto TraceEndLocation   = TraceStartLocation + TraceDirection * ShootingDistance;

    const auto MuzzleTransform     = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const auto MuzzleForwardVector = MuzzleTransform.GetRotation().GetForwardVector();

    auto HitResult = FHitResult{};
    auto CollisionParams = FCollisionQueryParams{};
    CollisionParams.AddIgnoredActor(Player);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation, ECC_Visibility, CollisionParams))
    {
        if (isTargetAhead(MuzzleForwardVector, HitResult.ImpactPoint - MuzzleTransform.GetLocation()))
        {
            DrawDebugLine(GetWorld(), MuzzleTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f,
                          0.0f, 3.0f);
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 10.0f, FColor::Red, false, 2.0f, 0.0f, 3.0f);
        }        
    }
    else
    {
        if (isTargetAhead(MuzzleForwardVector, TraceEndLocation - MuzzleTransform.GetLocation()))
        {
            DrawDebugLine(GetWorld(), MuzzleTransform.GetLocation(), TraceEndLocation, FColor::Green, false, 2.0f, 0.0f,
                          3.0f);
        }
    }
}
