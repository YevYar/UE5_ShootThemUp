// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/Components/STUWeaponComponent.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"

#include "Animations/STUAnimUtilities.h"
#include "Animations/STUEquipChangeWeaponAnimNotify.h"
#include "Animations/STUEquipWeaponFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Weapons/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

namespace
{
constexpr auto MaxWeaponsPerCharacterNumber = int32{2};
}

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::NextWeapon()
{
    if (CanEquipWeapon())
    {
        const auto NextWeaponIdx = (CurrentWeaponIndex + 1) % SpawnedWeapons.Num();
        if (NextWeaponIdx != CurrentWeaponIndex)
        {
            EquipTheWeapon(NextWeaponIdx);
        }
    }
}

void USTUWeaponComponent::ReloadCurrentWeapon()
{
    ReloadWeapon(CurrentWeapon);
}

void USTUWeaponComponent::ReloadWeapon(ASTUBaseWeapon* WeaponToReload)
{
    if (WeaponToReload != CurrentWeapon)
    {
        for (auto Weapon : SpawnedWeapons)
        {
            if (Weapon == WeaponToReload)
            {
                Weapon->ChangeClip();
                return;
            }
        }
        return;
    }

    if (CanReloadWeapon())
    {
        StopFire();

        if (PlayAnimMontage(CurrentReloadAnimMontage))
        {
            bIsReloadAnimationInProgress = true;
        }
    }
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

bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& WeaponAmmoData) const noexcept
{
    if (CurrentWeapon)
    {
        WeaponAmmoData = CurrentWeapon->GetWeaponAmmoData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const noexcept
{
    if (CurrentWeapon)
    {
        WeaponUIData = CurrentWeapon->GetWeaponUIData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !CurrentWeapon->IsAmmoEmpty() && !IsAnimMontageInProgress();
}

FVector USTUWeaponComponent::GetCurrentWeaponMuzzleLocation() const
{
    return CurrentWeapon ? CurrentWeapon->GetMuzzleLocation() : FVector{};
}

EWeaponType USTUWeaponComponent::GetCurrentWeaponType() const noexcept
{
    return CurrentWeapon ? CurrentWeapon->GetWeaponType() : EWeaponType::EWT_Unknown;
}

bool USTUWeaponComponent::DoesCurrentWeaponNeedAmmo() const
{
    return CurrentWeapon && !CurrentWeapon->IsAmmoFull();
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponClass, int32 ClipsAmount)
{
    for (const auto Weapon : SpawnedWeapons)
    {
        if (Weapon && Weapon->IsA(WeaponClass))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }

    return false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(WeaponDataToSpawn.Num() <= MaxWeaponsPerCharacterNumber, TEXT("Character can have no more than %d weapons!"),
           MaxWeaponsPerCharacterNumber);

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

bool USTUWeaponComponent::CanEquipWeapon() const
{
    return CurrentWeapon && !IsAnimMontageInProgress();
}

bool USTUWeaponComponent::CanReloadWeapon() const
{
    return CurrentWeapon && CurrentWeapon->CanReload() && !IsAnimMontageInProgress();
}

void USTUWeaponComponent::EquipTheWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= SpawnedWeapons.Num())
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Invalid weapon index %d"), WeaponIndex);
        return;
    }

    const auto Character = GetCharacter();
    if (!Character)
    {
        return;
    }

    StopFire();

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

bool USTUWeaponComponent::IsAnimMontageInProgress() const
{
    return bIsEquipAnimationInProgress || bIsReloadAnimationInProgress;
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
        if (auto EquipFinishedNotify =
              AnimUtilities::FindFirstAnimNotifyInAnimMontage<USTUEquipWeaponFinishedAnimNotify>(EquipWeaponMontage))
        {
            EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
        }
        else
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("EquipFinished anim notify was forgotten to set!"));
            checkNoEntry();
        }

        if (auto EquipChangeWeapon =
              AnimUtilities::FindFirstAnimNotifyInAnimMontage<USTUEquipChangeWeaponAnimNotify>(EquipWeaponMontage))
        {
            EquipChangeWeapon->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipChangeWeapon);
        }
        else
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("EquipChangeWeapon anim notify was forgotten to set!"));
            checkNoEntry();
        }
    }
}

UAnimMontage* USTUWeaponComponent::GetWeaponReloadAnimMontage(const UClass* WeaponClass)
{
    const auto WeaponData = WeaponDataToSpawn.FindByPredicate(
      [&](const FWeaponSpawnData& WeaponData)
      {
          return WeaponData.WeaponClass == WeaponClass;
      });
    return WeaponData ? WeaponData->ReloadAnimMontage : nullptr;
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

    CurrentWeapon            = SpawnedWeapons[NextWeaponIndex];
    CurrentReloadAnimMontage = GetWeaponReloadAnimMontage(CurrentWeapon->GetClass());
    CurrentWeaponIndex       = NextWeaponIndex;
    AttachWeaponToTheSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocket);
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    if (MeshComponent == GetCharacterMeshComponent())
    {
        bIsReloadAnimationInProgress = false;

        if (CurrentWeapon)
        {
            CurrentWeapon->ChangeClip();
        }
    }
}

void USTUWeaponComponent::SpawnWeapons()
{
    const auto Character = GetCharacter();
    if (!Character || !GetWorld())
    {
        return;
    }

    for (const auto WeaponData : WeaponDataToSpawn)
    {
        if (!WeaponData.ReloadAnimMontage)
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload Anim montage hasn't been provided."));
            continue;
        }

        auto SpawnedWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponData.WeaponClass);

        if (SpawnedWeapon)
        {
            AttachWeaponToTheSocket(SpawnedWeapon, Character->GetMesh(), WeaponArmourySocket);
            SpawnedWeapon->SetOwner(Character);

            SpawnedWeapon->ReloadRequired.AddUObject(this, &USTUWeaponComponent::ReloadWeapon);

            if (auto ReloadFinishedNotify =
                  AnimUtilities::FindFirstAnimNotifyInAnimMontage<USTUReloadFinishedAnimNotify>(WeaponData
                                                                                                  .ReloadAnimMontage))
            {
                ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
            }
            else
            {
                UE_LOG(LogWeaponComponent, Error, TEXT("ReloadFinished anim notify was forgotten to set!"));
                checkNoEntry();
            }

            SpawnedWeapons.Add(SpawnedWeapon);
        }
    }
}
