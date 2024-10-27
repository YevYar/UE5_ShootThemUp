// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS() class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
        GENERATED_BODY()

    public:
        // Sets default values for this character's properties
        ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer);

    protected:
        // Called when the game starts or when spawned
        virtual void BeginPlay() override;

    public:
        // Called every frame
        virtual void Tick(float DeltaTime) override;

        // Called to bind functionality to input
        virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
        void MoveForward(float Amount);
        void MoveRight(float Amount);
        void StartRun();
        void StopRun();
        void TryToJump();

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UCameraComponent* CameraComponent;
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USpringArmComponent* SpringArmComponent;
        bool                 bWantsToRun = false;

    private:
        bool     bIsIdleForward   = true;
        bool     bIsIdleRight     = true;
        // true - move forward, false - move backward, doesn't define if the Character is idle in the forward direction
        bool     bIsMovingForward = false;
        // true - move right, false - move left, doesn't define if the Character is idle in the right direction
        bool     bIsMovingRight   = false;
        FRotator InitialMeshRotation;
};
