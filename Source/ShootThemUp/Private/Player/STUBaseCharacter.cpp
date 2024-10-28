// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/STUBaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Player/Components/STUCharacterMovementComponent.h"
#include "Player/Components/STUHealthComponent.h"

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

    HealthComponent     = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    if (HealthTextComponent)
    {
        HealthTextComponent->SetupAttachment(GetRootComponent());
    }

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

    if (GetMesh())
    {
        InitialMeshRotation = GetMesh()->GetRelativeRotation();
    }
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), HealthComponent->GetHealth())));
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

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
    {
        return 0.0f;
    }

    const auto NormalizedVelocity = GetVelocity().GetSafeNormal();
    const auto AngleBetween       = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), NormalizedVelocity));
    const auto CrossProduct       = FVector::CrossProduct(GetActorForwardVector(), NormalizedVelocity);
    const auto Direction          = FMath::Sign(CrossProduct.Z);
    const auto Degrees            = FMath::RadiansToDegrees(AngleBetween);

    return CrossProduct.IsZero() ? Degrees : Direction * Degrees;
}

bool ASTUBaseCharacter::IsJumping() const
{
    return GetCharacterMovement() && GetCharacterMovement()->IsFalling();
}

bool ASTUBaseCharacter::IsIdle() const
{
    return bIsIdleForward && bIsIdleRight && GetVelocity().IsZero();
}

bool ASTUBaseCharacter::IsMovingBackward() const
{
    return !bIsIdleForward && !bIsMovingForward && !GetVelocity().IsZero();
}

bool ASTUBaseCharacter::IsMovingForward() const
{
    return !bIsIdleForward && bIsMovingForward && !GetVelocity().IsZero();
}

bool ASTUBaseCharacter::IsMovingLeft() const
{
    return !bIsIdleRight && !bIsMovingRight && !GetVelocity().IsZero();
}

bool ASTUBaseCharacter::IsMovingRight() const
{
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
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);

    bIsIdleRight   = Amount == 0.0f;
    bIsMovingRight = Amount > NULL_EPSILON;

    const auto Direction        = GetMovementDirection();
    const auto AbsolutDirection = FMath::Abs(Direction);

    if (IsRunning() && GetMesh() && (AbsolutDirection > 0.0f && AbsolutDirection < 90.0f))
    {
        auto NewRotation = GetMesh()->GetRelativeRotation();
        NewRotation.Yaw  = InitialMeshRotation.Yaw + Direction;
        GetMesh()->SetRelativeRotation(NewRotation);
    }
    else if (GetMesh())
    {
        GetMesh()->SetRelativeRotation(InitialMeshRotation);
    }
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
