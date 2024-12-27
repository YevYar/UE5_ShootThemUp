// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
        GENERATED_BODY()

    public:
        USTUWeaponComponent();

        UFUNCTION(BlueprintCallable, Category = "Damage")
        void StartFire();
        UFUNCTION(BlueprintCallable, Category = "Damage")
        void StopFire();

        void SetLifeSpan(float LifeSpan);

    protected:
        virtual void BeginPlay() override;

    private:
        void SpawnWeapon();

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        TSubclassOf<ASTUBaseWeapon> WeaponClass;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        FName WeaponAttachementPoint = "WeaponPoint";

        UPROPERTY()
        ASTUBaseWeapon* SpawnedWeapon;
};
