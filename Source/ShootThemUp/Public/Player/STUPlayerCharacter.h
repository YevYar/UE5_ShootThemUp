// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class UCameraComponent;
class USphereComponent;
class USpringArmComponent;

UCLASS() class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
        GENERATED_BODY()

    public:
        ASTUPlayerCharacter(const FObjectInitializer& ObjectInitializer);

        bool IsRunning() const override;
        void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

    protected:
        void BeginPlay() override;
        void ResetFields() override;
        void OnDeath() override;

        UFUNCTION()
        void OnCameraBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult);
        UFUNCTION()
        void OnCameraEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

        void MoveForward(float Amount);
        void MoveRight(float Amount);
        void StartRun();
        void StopRun();
        void TryToJump();

        bool CheckIfCameraOverlapCharacter(const UPrimitiveComponent* OverlappedComponent) const;
        void SetCharacterVisibilityForPlayer(bool Visibility);

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USphereComponent* CameraCollisionComponent;
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UCameraComponent* CameraComponent;
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USpringArmComponent* SpringArmComponent;

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
