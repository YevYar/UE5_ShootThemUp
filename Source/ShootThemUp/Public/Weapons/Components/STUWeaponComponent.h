// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "Animations/STUBaseAnimNotify.h"
#include "STUCoreTypes.h"

#include "STUWeaponComponent.generated.h"

class ACharacter;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
        GENERATED_BODY()

    public:
        USTUWeaponComponent();

        UFUNCTION(BlueprintCallable, Category = "Actions")
        virtual void NextWeapon();
        UFUNCTION(BlueprintCallable, Category = "Actions")
        void ReloadCurrentWeapon();
        UFUNCTION(BlueprintCallable, Category = "Actions")
        void ReloadWeapon(ASTUBaseWeapon* WeaponToReload);

        UFUNCTION(BlueprintCallable, Category = "Damage")
        virtual void StartFire();
        UFUNCTION(BlueprintCallable, Category = "Damage")
        virtual void StopFire();

        UFUNCTION(BlueprintCallable, Category = "UI")
        bool GetCurrentWeaponAmmoData(FAmmoData& WeaponAmmoData) const noexcept;
        UFUNCTION(BlueprintCallable, Category = "UI")
        bool GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const noexcept;

        bool        CanFire() const;
        FVector     GetCurrentWeaponMuzzleLocation() const;
        EWeaponType GetCurrentWeaponType() const noexcept;
        bool        DoesCurrentWeaponNeedAmmo() const;
        bool        TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponClass, int32 ClipsAmount);

    protected:
        void BeginPlay() override;
        void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

        virtual void SpawnWeapons();

        void        AttachWeaponToTheSocket(ASTUBaseWeapon* Weapon, USceneComponent* Mesh, const FName& SocketName);
        bool        CanEquipWeapon() const;
        bool        CanReloadWeapon() const;
        void        EquipTheWeapon(int32 WeaponIndex);
        ACharacter* GetCharacter() const;
        USkeletalMeshComponent* GetCharacterMeshComponent() const;
        bool                    IsAnimMontageInProgress() const;
        bool                    PlayAnimMontage(UAnimMontage* AnimMontage);

    private:
        virtual void SubscribeOnNotifiers();

        UAnimMontage* GetWeaponReloadAnimMontage(const UClass* WeaponClass);
        void          OnEquipFinished(USkeletalMeshComponent* MeshComponent);
        void          OnEquipChangeWeapon(USkeletalMeshComponent* MeshComponent);
        void          OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
        UAnimMontage* EquipWeaponMontage;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        TArray<FWeaponSpawnData> WeaponDataToSpawn;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        FName WeaponArmourySocket = "WeaponArmouryPoint";
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        FName WeaponEquipSocket = "WeaponEquipPoint";

        UPROPERTY()
        TArray<ASTUBaseWeapon*> SpawnedWeapons;
        UPROPERTY()
        ASTUBaseWeapon* CurrentWeapon            = nullptr;
        UAnimMontage*   CurrentReloadAnimMontage = nullptr;
        int32           CurrentWeaponIndex       = 0;
        int32           NextWeaponIndex          = 0;

    private:
        bool bIsEquipAnimationInProgress  = false;
        bool bIsReloadAnimationInProgress = false;
};
