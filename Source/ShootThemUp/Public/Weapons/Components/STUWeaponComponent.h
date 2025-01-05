// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUWeaponComponent.generated.h"

class ACharacter;
class ASTUBaseWeapon;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
        GENERATED_BODY()

    public:
        USTUWeaponComponent();

        UFUNCTION(BlueprintCallable, Category = "Actions")
        void NextWeapon();

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
        void        EquipTheWeapon(int32 WeaponIndex);
        ACharacter* GetCharacter() const;
        USkeletalMeshComponent* GetCharacterMeshComponent() const;
        bool                    PlayAnimMontage(UAnimMontage* AnimMontage);

    private:
        virtual void SubscribeOnNotifiers();

        void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
        void OnEquipChangeWeapon(USkeletalMeshComponent* MeshComponent);
        void SpawnWeapons();

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
        UAnimMontage* EquipWeaponMontage;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        TArray<TSubclassOf<ASTUBaseWeapon>> WeaponClassesToSpawn;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        FName WeaponArmourySocket = "WeaponArmouryPoint";
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        FName WeaponEquipSocket = "WeaponEquipPoint";

        UPROPERTY()
        TArray<ASTUBaseWeapon*> SpawnedWeapons;
        UPROPERTY()
        ASTUBaseWeapon* CurrentWeapon      = nullptr;
        int32           CurrentWeaponIndex = 0;
        int32           NextWeaponIndex    = 0;

    private:
        bool bIsEquipAnimationInProgress = false;
};
