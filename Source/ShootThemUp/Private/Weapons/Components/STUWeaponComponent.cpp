// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/Components/STUWeaponComponent.h"

#include "GameFramework/Character.h"

#include "Weapons/STUBaseWeapon.h"

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::Fire()
{
    if (SpawnedWeapon)
    {
        SpawnedWeapon->Fire();
    }
}

void USTUWeaponComponent::SetLifeSpan(float LifeSpan)
{
    if (SpawnedWeapon)
    {
        SpawnedWeapon->SetLifeSpan(LifeSpan);
    }
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapon();

    check(SpawnedWeapon);
}

void USTUWeaponComponent::SpawnWeapon()
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld())
    {
        return;
    }

    SpawnedWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
    if (SpawnedWeapon)
    {
        SpawnedWeapon->AttachToComponent(Character->GetMesh(),
                                         FAttachmentTransformRules{EAttachmentRule::SnapToTarget, false},
                                         WeaponAttachementPoint);
        SpawnedWeapon->SetOwner(Character);
    }
}
