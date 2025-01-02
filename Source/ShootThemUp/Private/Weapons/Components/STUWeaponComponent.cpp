// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/Components/STUWeaponComponent.h"

#include "GameFramework/Character.h"

#include "Weapons/STUBaseWeapon.h"

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::NextWeapon()
{
    const auto NextWeaponIndex = (CurrentWeaponIndex + 1) % SpawnedWeapons.Num();
    EquipTheWeapon(NextWeaponIndex);
}

void USTUWeaponComponent::StartFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StartFire();
    }
}

void USTUWeaponComponent::StopFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
    }
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapons();
    EquipTheWeapon(0);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    StopFire();

    CurrentWeapon = nullptr;

    for (auto Weapon : SpawnedWeapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }

    SpawnedWeapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::AttachWeaponToTheSocket(ASTUBaseWeapon* Weapon, USceneComponent* Mesh,
                                                  const FName& SocketName)
{
    if (Weapon && Mesh)
    {
        Weapon->AttachToComponent(Mesh, FAttachmentTransformRules{EAttachmentRule::SnapToTarget, false}, SocketName);
    }
}

void USTUWeaponComponent::EquipTheWeapon(int32 WeaponIndex)
{
    if (WeaponIndex >= SpawnedWeapons.Num())
    {
        return;
    }

    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return;
    }

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToTheSocket(CurrentWeapon, Character->GetMesh(), WeaponArmourySocket);
    }

    CurrentWeapon      = SpawnedWeapons[WeaponIndex];
    CurrentWeaponIndex = WeaponIndex;
    AttachWeaponToTheSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocket);
}

void USTUWeaponComponent::SpawnWeapons()
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld())
    {
        return;
    }

    for (const auto WeaponClass : WeaponClassesToSpawn)
    {
        auto SpawnedWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);

        if (SpawnedWeapon)
        {
            AttachWeaponToTheSocket(SpawnedWeapon, Character->GetMesh(), WeaponArmourySocket);
            SpawnedWeapon->SetOwner(Character);

            SpawnedWeapons.Add(SpawnedWeapon);
        }
    }
}
