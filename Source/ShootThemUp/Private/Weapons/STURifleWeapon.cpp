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
    if (IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    ACharacter*  Player     = nullptr;
    AController* Controller = nullptr;
    if (!GetPlayerAndController(Player, Controller))
    {
        StopFire();
        return;
    }

    auto TraceStartLocation = FVector{};
    auto TraceEndLocation   = FVector{};
    if (!GetTraceData(TraceStartLocation, TraceEndLocation))
    {
        StopFire();
        return;
    }

    UE_LOG(LogTemp, Error, TEXT("Rifle shot!"));

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

    DecreaseBullets();
}

float ASTURifleWeapon::CalculateDamage(float DistanceFromMuzzle, float DistanceFromTraceStartToMuzzle) const
{
    return FMath::GetMappedRangeValueClamped(FVector2D{100.0f, ShootingDistance - DistanceFromTraceStartToMuzzle},
                                             FVector2D{MaxDamage, MinDamage}, DistanceFromMuzzle);
}

FVector ASTURifleWeapon::GetTraceDirection(const FVector& ViewPointForwardVector) const
{
    const auto ShootingConeHalfRad = FMath::DegreesToRadians(ShootingSpreadConeAngle) / 2.0f;
    return FMath::VRandCone(ViewPointForwardVector, FMath::Clamp(TimeFromFireStart / 4, 0.0f, ShootingConeHalfRad));
}
