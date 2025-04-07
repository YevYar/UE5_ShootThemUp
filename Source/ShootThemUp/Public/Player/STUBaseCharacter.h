// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UAnimMontage;
class USTUHealthComponent;
class USTUWeaponComponent;
class UTextRenderComponent;

UCLASS() class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
        GENERATED_BODY()

    public:
        ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer);

        void Tick(float DeltaTime) override;

        UFUNCTION(BlueprintCallable, Category = "Life State")
        bool IsDead() const;

        UFUNCTION(BlueprintCallable, Category = "Movement")
        float GetMovementDirection() const;
        UFUNCTION(BlueprintCallable, Category = "Movement")
        bool IsJumping() const;
        UFUNCTION(BlueprintCallable, Category = "Movement")
        virtual bool IsRunning() const;

        void SetPlayerColor(const FLinearColor& PlayerColor);

    protected:
        void BeginPlay() override;
        void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

        UFUNCTION()
        virtual void OnDeath();
        UFUNCTION()
        void OnHealthChanged(float NewHealth, bool IsCausedByDamage = false, float LastDamage = 0.0f);
        UFUNCTION()
        void OnLanding(const FHitResult& LandingHit);

        virtual void ResetFields();

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Appearance",
                  meta = (ClampMin = "0.0", ToolTip = "Time in seconds."))
        float LifeSpanAfterDeath = 10.0f;

        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UTextRenderComponent* DamageTextComponent;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UAnimMontage* DeathMontage;
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USTUHealthComponent* HealthComponent;
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UTextRenderComponent* HealthTextComponent;
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USTUWeaponComponent* WeaponComponent;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        FVector2D LandingDamage = FVector2D{10.0f, 100.0f};
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        FVector2D LandingDamageVelocity = FVector2D{900.0f, 1200.0f};

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
        FName MaterialColorName = "Paint Color";

    private:
        FTimerHandle DamageDisplayTimer;
};
