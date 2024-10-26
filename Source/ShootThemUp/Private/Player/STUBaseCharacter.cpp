// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/STUBaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Player/Components/STUCharacterMovementComponent.h"

namespace
{
constexpr inline float NULL_EPSILON = 0.0001f;
}  // namespace

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer
            .SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    if (!SpringArmComponent)
    {
        return;
    }

    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetupAttachment(GetRootComponent());

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    if (CameraComponent)
    {
        CameraComponent->SetupAttachment(SpringArmComponent);
    }
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
        PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
        PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
        PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::StartRun);
        PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::StopRun);
        PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::TryToJump);
    }
}

bool ASTUBaseCharacter::IsJumping() const
{
    return GetCharacterMovement() && GetCharacterMovement()->IsFalling();
}

bool ASTUBaseCharacter::IsIdle() const
{
    // return GetVelocity().IsNearlyZero(NULL_EPSILON);  // !bIsMovingForward && !bIsMovingRight;
    return bIsIdleForward && bIsIdleRight && GetVelocity().IsZero();
}

bool ASTUBaseCharacter::IsMovingBackward() const
{
    // return /*bIsMovingForward && */ FVector::DotProduct(GetActorForwardVector(), GetVelocity()) < -1.0f;
    return !bIsIdleForward && !bIsMovingForward && !GetVelocity().IsZero();
}

bool ASTUBaseCharacter::IsMovingForward() const
{
    PrintVectors();
    // return /*bIsMovingForward && */ FVector::DotProduct(GetActorForwardVector(), GetVelocity()) > 1.0f;
    return !bIsIdleForward && bIsMovingForward && !GetVelocity().IsZero();
}

bool ASTUBaseCharacter::IsMovingLeft() const
{
    // return /*bIsMovingRight && */ FVector::DotProduct(GetActorRightVector(), GetVelocity()) < -1.0f;
    return !bIsIdleRight && !bIsMovingRight && !GetVelocity().IsZero();
}

bool ASTUBaseCharacter::IsMovingRight() const
{
    // return /*bIsMovingRight && */ FVector::DotProduct(GetActorRightVector(), GetVelocity()) > 1.0f;
    return !bIsIdleRight && bIsMovingRight && !GetVelocity().IsZero();
}

bool ASTUBaseCharacter::IsRunning() const
{
    return bWantsToRun && IsMovingForward();
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount);

    bIsIdleForward   = Amount == 0.0f;
    bIsMovingForward = Amount > NULL_EPSILON;

    // bIsMovingForward = Amount > NULL_EPSILON;

    // if (GetCharacterMovement())
    //{
    //     GetCharacterMovement()->MaxWalkSpeed = IsRunning() ? SPRINTING_SPEED : NORMAL_WALK_SPEED;
    // }
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);

    bIsIdleRight   = Amount == 0.0f;
    bIsMovingRight = Amount > NULL_EPSILON;

    // bIsMovingRight = Amount > NULL_EPSILON;
}

void ASTUBaseCharacter::StartRun()
{
    bWantsToRun = true;
}

void ASTUBaseCharacter::StopRun()
{
    bWantsToRun = false;
}

void ASTUBaseCharacter::TryToJump()
{
    if (IsIdle() || IsMovingForward())
    {
        Jump();
    }
}

void ASTUBaseCharacter::PrintVectors() const
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
                                         FString::Printf(TEXT("Forward vector: %s"),
                                                         *GetActorForwardVector().ToString()));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
                                         FString::Printf(TEXT("Velocity vector: %s"), *GetVelocity().ToString()));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
                                         FString::Printf(TEXT("Dot product: %f"),
                                                         FVector::DotProduct(GetActorForwardVector(), GetVelocity())));
    }
}
