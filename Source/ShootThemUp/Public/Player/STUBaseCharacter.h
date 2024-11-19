// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class ASTUBaseWeapon;
class UAnimMontage;
class UCameraComponent;
class USTUHealthComponent;
class USpringArmComponent;
class UTextRenderComponent;

UCLASS() class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
        GENERATED_BODY()

    public:
        ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer);

    public:
        virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
        virtual void Tick(float DeltaTime) override;

        UFUNCTION(BlueprintCallable, Category = "Life State")
        bool IsDead() const;

        UFUNCTION(BlueprintCallable, Category = "Movement")
        float GetMovementDirection() const;
        UFUNCTION(BlueprintCallable, Category = "Movement")
        bool IsJumping() const;
        UFUNCTION(BlueprintCallable, Category = "Movement")
        bool IsIdle() const;
        UFUNCTION(BlueprintCallable, Category = "Movement")
        bool IsMovingBackward() const;
        UFUNCTION(BlueprintCallable, Category = "Movement")
        bool IsMovingForward() const;
        UFUNCTION(BlueprintCallable, Category = "Movement")
        bool IsMovingLeft() const;
        UFUNCTION(BlueprintCallable, Category = "Movement")
        bool IsMovingRight() const;
        UFUNCTION(BlueprintCallable, Category = "Movement")
        bool IsRunning() const;

    protected:
        virtual void BeginPlay() override;
        void         MoveForward(float Amount);
        void         MoveRight(float Amount);
        void         StartRun();
        void         StopRun();
        void         TryToJump();

        UFUNCTION()
        void OnDeath();
        UFUNCTION()
        void OnHealthChanged(float NewHealth);
        UFUNCTION()
        void OnLanding(const FHitResult& LandingHit);

    private:
        void SpawnWeapon();

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Appearance", meta = (ToolTip = "Time in seconds."))
        float LifeSpanAfterDeath = 10.0f;

        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UCameraComponent* CameraComponent;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UAnimMontage* DeathMontage;
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USTUHealthComponent* HealthComponent;
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UTextRenderComponent* HealthTextComponent;
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USpringArmComponent* SpringArmComponent;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        FVector2D LandingDamage = FVector2D{10.0f, 100.0f};
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        FVector2D LandingDamageVelocity = FVector2D{900.0f, 1200.0f};

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        TSubclassOf<ASTUBaseWeapon> WeaponClass;

        ASTUBaseWeapon* SpawnedWeapon;

    private:
        bool     bIsIdleForward   = true;
        bool     bIsIdleRight     = true;
        // true - move forward, false - move backward, doesn't define if the Character is idle in the forward direction
        bool     bIsMovingForward = false;
        // true - move right, false - move left, doesn't define if the Character is idle in the right direction
        bool     bIsMovingRight   = false;
        bool     bWantsToRun      = false;
        FRotator InitialMeshRotation;
};
