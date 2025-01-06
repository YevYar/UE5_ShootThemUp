// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "Animations/STUBaseAnimNotify.h"

#include "STUWeaponComponent.generated.h"

class ACharacter;
class ASTUBaseWeapon;
class UAnimMontage;

USTRUCT(BlueprintType) struct FWeaponSpawnData
{
        GENERATED_USTRUCT_BODY()

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        TSubclassOf<ASTUBaseWeapon> WeaponClass;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSettings")
        UAnimMontage* ReloadAnimMontage = nullptr;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
        GENERATED_BODY()

    public:
        USTUWeaponComponent();

        UFUNCTION(BlueprintCallable, Category = "Actions")
        void NextWeapon();
        UFUNCTION(BlueprintCallable, Category = "Actions")
        void ReloadWeapon();

        UFUNCTION(BlueprintCallable, Category = "Damage")
        void StartFire();
        UFUNCTION(BlueprintCallable, Category = "Damage")
        void StopFire();

    protected:
        virtual void BeginPlay() override;
        virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

        void        AttachWeaponToTheSocket(ASTUBaseWeapon* Weapon, USceneComponent* Mesh, const FName& SocketName);
        bool        CanFire() const;
        bool        CanEquipWeapon() const;
        bool        CanReloadWeapon() const;
        void        EquipTheWeapon(int32 WeaponIndex);
        ACharacter* GetCharacter() const;
        USkeletalMeshComponent* GetCharacterMeshComponent() const;
        bool                    IsAnimMontageInProgress() const;
        bool                    PlayAnimMontage(UAnimMontage* AnimMontage);

    private:
        template<typename NotifyType>
        requires std::is_base_of_v<USTUBaseAnimNotify, NotifyType>
        static NotifyType* FindFirstAnimNotifyInAnimMontage(const UAnimMontage* AnimMontage)
        {
            if (!AnimMontage)
            {
                return nullptr;
            }

            for (const auto& Notify : AnimMontage->Notifies)
            {
                if (auto CastedNotify = Cast<NotifyType>(Notify.Notify))
                {
                    return CastedNotify;
                }
            }

            return nullptr;
        }

        virtual void SubscribeOnNotifiers();

        UAnimMontage* GetWeaponReloadAnimMontage(const UClass* WeaponClass);
        void          OnEquipFinished(USkeletalMeshComponent* MeshComponent);
        void          OnEquipChangeWeapon(USkeletalMeshComponent* MeshComponent);
        void          OnReloadFinished(USkeletalMeshComponent* MeshComponent);
        void          SpawnWeapons();

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
