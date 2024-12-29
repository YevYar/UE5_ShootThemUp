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

void ASTUBaseWeapon::StartFire()
{
    UE_LOG(LogBaseWeapon, All, TEXT("Start Fire!"));
}

void ASTUBaseWeapon::StopFire()
{
    UE_LOG(LogBaseWeapon, All, TEXT("Stop Fire!"));
}

bool ASTUBaseWeapon::IsTargetAhead(const FVector& MuzzleForwardVector, const FVector& Target)
{
    return FVector::DotProduct(MuzzleForwardVector, Target) > 0.0f;
}

FVector ASTUBaseWeapon::GetShotDirection(const FVector_NetQuantize& ImpactPoint, const FVector& MuzzleLocation)
{
    return (ImpactPoint - MuzzleLocation).GetSafeNormal();
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
}

void ASTUBaseWeapon::ApplyDamageToTheHitActor(const FHitResult& HitResult, const FVector& MuzzleLocation) const
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
