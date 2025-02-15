// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/STURifleWeapon.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Weapons/Components/STUWeaponVFXComponent.h"

ASTURifleWeapon::ASTURifleWeapon()
{
    VFXComponent = CreateDefaultSubobject<USTUWeaponVFXComponent>("VFXComponent");
}

void ASTURifleWeapon::StartFire()
{
    TimeFromFireStart = 0.0f;
    GetWorldTimerManager().SetTimer(BurstShootingTimer, this, &ASTURifleWeapon::MakeShotTimerSlot, ShootingInterval,
                                    true);
    if (MakeShot())
    {
        InitMuzzleEffect();
    }
}

void ASTURifleWeapon::StopFire()
{
    if (BurstShootingTimer.IsValid())
    {
        TimeFromFireStart = 0.0f;
        GetWorldTimerManager().ClearTimer(BurstShootingTimer);
    }
    SetMuzzleEffectVisibility(false);
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(VFXComponent);
}

bool ASTURifleWeapon::MakeShot()
{
    if (IsAmmoEmpty())
    {
        StopFire();
        return false;
    }

    ACharacter*  Player     = nullptr;
    AController* Controller = nullptr;
    if (!GetPlayerAndController(Player, Controller))
    {
        StopFire();
        return false;
    }

    auto TraceStartLocation = FVector{};
    auto TraceEndLocation   = FVector{};
    if (!GetTraceData(TraceStartLocation, TraceEndLocation))
    {
        StopFire();
        return false;
    }

    UE_LOG(LogTemp, Error, TEXT("Rifle shot!"));

    const auto MuzzleTransform     = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const auto MuzzleForwardVector = MuzzleTransform.GetRotation().GetForwardVector();

    auto HitResult       = FHitResult{};
    auto CollisionParams = FCollisionQueryParams{};
    CollisionParams.AddIgnoredActor(Player);
    CollisionParams.bReturnPhysicalMaterial = true;

    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation, ECC_Visibility,
                                             CollisionParams))
    {
        if (IsTargetAhead(MuzzleForwardVector, HitResult.ImpactPoint - MuzzleTransform.GetLocation()))
        {
            TraceEndLocation = HitResult.ImpactPoint;
            /*DrawDebugLine(GetWorld(), MuzzleTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f,
                          0.0f, 3.0f);*/
            VFXComponent->PlayImpactVFX(HitResult);

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
            /*DrawDebugLine(GetWorld(), MuzzleTransform.GetLocation(), TraceEndLocation, FColor::Green, false, 2.0f, 0.0f,
                          3.0f);*/
        }
    }

    SpawnTraceEffect(MuzzleTransform.GetLocation(), TraceEndLocation);

    TimeFromFireStart += ShootingInterval;

    DecreaseBullets();

    return true;
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

void ASTURifleWeapon::InitMuzzleEffect()
{
    if (!MuzzleEffectComponent)
    {
        MuzzleEffectComponent = SpawnMuzzleEffect();
    }
    SetMuzzleEffectVisibility(true);
}

void ASTURifleWeapon::MakeShotTimerSlot()
{
    MakeShot();
}

void ASTURifleWeapon::SetMuzzleEffectVisibility(bool Visibility)
{
    if (MuzzleEffectComponent)
    {
        MuzzleEffectComponent->SetPaused(!Visibility);
        MuzzleEffectComponent->SetVisibility(Visibility, true);
    }
}

void ASTURifleWeapon::SpawnTraceEffect(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceEffectComponent =
      UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceTargetEffect, TraceStart);
    if (TraceEffectComponent)
    {
        TraceEffectComponent->SetVariableVec3(TraceTargetName, TraceEnd);
    }
}
