// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/STUBaseWeapon.h"

#include "Components/SkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::Fire()
{
    UE_LOG(LogBaseWeapon, All, TEXT("Fire!"));
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
}

bool ASTUBaseWeapon::isTargetAhead(const FVector& MuzzleForwardVector, const FVector& Target) const
{
    return FVector::DotProduct(MuzzleForwardVector, Target) > 0.0f;
}
