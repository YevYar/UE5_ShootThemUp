// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;
class UPhysicalMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
        GENERATED_BODY()

    public:
        USTUHealthComponent();

        UFUNCTION(BlueprintCallable, Category = "Health")
        float GetHealthPercent() const noexcept;

        float GetHealth() const noexcept;
        bool  IsDead() const noexcept;
        bool  IsHealthFull() const noexcept;
        void  SetHealth(float NewHealth, bool IsCausedByDamage = false, AController* DamageCauser = nullptr,
                        float LastDamage = 0.0f) noexcept;
        bool  TryToAddHealth(float HealthAmount);

    protected:
        void BeginPlay() override;

    private:
        UFUNCTION()
        void AutoHeal();
        UFUNCTION()
        void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                             class AController* InstigatedBy, AActor* DamageCauser);
        UFUNCTION()
        void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
                               class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                               const class UDamageType* DamageType, AActor* DamageCauser);
        UFUNCTION()
        void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin,
                                const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

        void  ApplyDamage(float Damage, AController* InstigatedBy);
        float GetPointDamageModifier(const AActor* DamagedActor, const FName& BoneName);
        void  Killed(const AController* KillerController) const;
        void  PlayCameraShakeEffect(TSubclassOf<UCameraShakeBase> CameraShakeEffect);
        void  ReportDamageEvent(float DamageAmount, AController* InstigatedBy);
        void  StopHealing();

    public:
        UPROPERTY(BlueprintAssignable, Category = "Health")
        FDiedSignature Died;
        UPROPERTY(BlueprintAssignable, Category = "Health")
        FHeadshotSignature GotHeadshot;
        UPROPERTY(BlueprintAssignable, Category = "Health")
        FHealthChangedSignature HealthChanged;

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        TMap<UPhysicalMaterial*, float> DamageModifiersMap;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        FName HeadBoneName = "b_head";
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        float RequiredLandingDamageToShowLandingEffect = 20.0f;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
        bool AutoHealEnabled = true;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",
                  meta = (ClampMin = "0.1", EditCondition = "AutoHealEnabled"))
        float HealAmount = 0.1f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",
                  meta = (ClampMin = "0.1", EditCondition = "AutoHealEnabled"))
        float HealDelay = 0.1f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",
                  meta = (ClampMin = "0.1", EditCondition = "AutoHealEnabled"))
        float HealInterval = 1.0f;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0"))
        float MaxHealth = 100.0f;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        TSubclassOf<UCameraShakeBase> CameraShakeOnDamageEffect;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        TSubclassOf<UCameraShakeBase> CameraShakeOnLandingEffect;

    private:
        bool         bIsDead = false;
        FTimerHandle HealTimer;
        float        Health = 0.0f;
};
