// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChanged, float, Health);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
        GENERATED_BODY()

    public:
        // Sets default values for this component's properties
        USTUHealthComponent();

        float GetHealth() const noexcept;
        bool  IsDead() const noexcept;
        void  SetHealth(float NewHealth) noexcept;

    protected:
        // Called when the game starts
        virtual void BeginPlay() override;

    private:
        UFUNCTION()
        void AutoHeal();
        UFUNCTION()
        void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                             class AController* InstigatedBy, AActor* DamageCauser);
        void StopHealing();

    public:
        UPROPERTY(BlueprintAssignable, Category = "Health")
        FDied Died;
        UPROPERTY(BlueprintAssignable, Category = "Health")
        FHealthChanged HealthChanged;

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
        bool AutoHealEnabled = true;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (ClampMin = "0.1"))
        float HealAmount = 0.1f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (ClampMin = "0.1"))
        float HealDelay = 0.1f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (ClampMin = "0.1"))
        float HealInterval = 1.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0"))
        float MaxHealth = 100.0f;

    private:
        bool         bIsDead = false;
        FTimerHandle HealTimer;
        float        Health = 0.0f;
};
