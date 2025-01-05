// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/Components/STUWeaponComponent.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"

#include "Animations/STUEquipChangeWeaponAnimNotify.h"
#include "Animations/STUEquipWeaponFinishedAnimNotify.h"
#include "Weapons/STUBaseWeapon.h"

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::NextWeapon()
{
    if (!CanEquipWeapon())
    {
        return;
    }

    EquipTheWeapon((CurrentWeaponIndex + 1) % SpawnedWeapons.Num());
}

void USTUWeaponComponent::StartFire()
{
    if (CanFire())
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
    SubscribeOnNotifiers();
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

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !bIsEquipAnimationInProgress;
}

bool USTUWeaponComponent::CanEquipWeapon() const
{
    return !bIsEquipAnimationInProgress;
}

void USTUWeaponComponent::EquipTheWeapon(int32 WeaponIndex)
{
    if (WeaponIndex >= SpawnedWeapons.Num())
    {
        return;
    }

    const auto Character = GetCharacter();
    if (!Character)
    {
        return;
    }

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
    }

    NextWeaponIndex = WeaponIndex;

    if (PlayAnimMontage(EquipWeaponMontage))
    {
        bIsEquipAnimationInProgress = true;
    }
}

ACharacter* USTUWeaponComponent::GetCharacter() const
{
    return Cast<ACharacter>(GetOwner());
}

USkeletalMeshComponent* USTUWeaponComponent::GetCharacterMeshComponent() const
{
    auto Character = GetCharacter();
    if (Character)
    {
        return Character->GetMesh();
    }

    return nullptr;
}

bool USTUWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{
    auto Character = GetCharacter();
    if (Character)
    {
        return Character->PlayAnimMontage(AnimMontage) > 0.0f;
    }

    return false;
}

void USTUWeaponComponent::SubscribeOnNotifiers()
{
    check(EquipWeaponMontage);

    if (EquipWeaponMontage)
    {
        for (const auto& Notify : EquipWeaponMontage->Notifies)
        {
            if (auto EquipFinishedNotify = Cast<USTUEquipWeaponFinishedAnimNotify>(Notify.Notify))
            {
                EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            }
            else if (auto EquipChangeWeapon = Cast<USTUEquipChangeWeaponAnimNotify>(Notify.Notify))
            {
                EquipChangeWeapon->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipChangeWeapon);
            }
        }
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    if (MeshComponent == GetCharacterMeshComponent())
    {
        bIsEquipAnimationInProgress = false;
    }
}

void USTUWeaponComponent::OnEquipChangeWeapon(USkeletalMeshComponent* MeshComponent)
{
    const auto Character = GetCharacter();
    if (!Character || MeshComponent != Character->GetMesh())
    {
        return;
    }

    if (CurrentWeapon)
    {
        AttachWeaponToTheSocket(CurrentWeapon, Character->GetMesh(), WeaponArmourySocket);
    }

    CurrentWeapon      = SpawnedWeapons[NextWeaponIndex];
    CurrentWeaponIndex = NextWeaponIndex;
    AttachWeaponToTheSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocket);
}

void USTUWeaponComponent::SpawnWeapons()
{
    const auto Character = GetCharacter();
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
