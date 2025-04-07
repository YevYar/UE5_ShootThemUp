// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/STUPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Weapons/Components/STUWeaponComponent.h"

namespace
{
constexpr inline float NULL_EPSILON = 0.0001f;
}  // namespace

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    if (!SpringArmComponent)
    {
        return;
    }

    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->SocketOffset    = FVector{0.0f, 100.0f, 120.0f};
    SpringArmComponent->TargetArmLength = 400;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    if (CameraComponent)
    {
        CameraComponent->SetupAttachment(SpringArmComponent);
    }
}

bool ASTUPlayerCharacter::IsRunning() const
{
    return bWantsToRun && IsMovingForward();
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);
        PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::AddControllerPitchInput);
        PlayerInputComponent->BindAxis("TurnAround", this, &ASTUPlayerCharacter::AddControllerYawInput);
        PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::StartRun);
        PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::StopRun);
        PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUPlayerCharacter::TryToJump);
        PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
        PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
        PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
        PlayerInputComponent->BindAction("ReloadWeapon", IE_Pressed, WeaponComponent,
                                         &USTUWeaponComponent::ReloadCurrentWeapon);
    }
}

bool ASTUPlayerCharacter::IsIdle() const
{
    return bIsIdleForward && bIsIdleRight && GetVelocity().IsZero();
}

bool ASTUPlayerCharacter::IsMovingBackward() const
{
    return !bIsIdleForward && !bIsMovingForward && !GetVelocity().IsZero();
}

bool ASTUPlayerCharacter::IsMovingForward() const
{
    return !bIsIdleForward && bIsMovingForward && !GetVelocity().IsZero();
}

bool ASTUPlayerCharacter::IsMovingLeft() const
{
    return !bIsIdleRight && !bIsMovingRight && !GetVelocity().IsZero();
}

bool ASTUPlayerCharacter::IsMovingRight() const
{
    return !bIsIdleRight && bIsMovingRight && !GetVelocity().IsZero();
}

void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (GetMesh())
    {
        InitialMeshRotation = GetMesh()->GetRelativeRotation();
    }
}

void ASTUPlayerCharacter::ResetFields()
{
    Super::ResetFields();

    bIsIdleForward      = true;
    bIsIdleRight        = true;
    bIsMovingForward    = false;
    bIsMovingRight      = false;
    bWantsToRun         = false;
    InitialMeshRotation = FRotator{};
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUPlayerCharacter::MoveForward(float Amount)
{
    if (IsDead())
    {
        return;
    }

    AddMovementInput(GetActorForwardVector(), Amount);

    bIsIdleForward   = Amount == 0.0f;
    bIsMovingForward = Amount > NULL_EPSILON;
}

void ASTUPlayerCharacter::MoveRight(float Amount)
{
    if (IsDead())
    {
        return;
    }

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

void ASTUPlayerCharacter::StartRun()
{
    if (IsDead())
    {
        return;
    }

    bWantsToRun = true;
}

void ASTUPlayerCharacter::StopRun()
{
    if (IsDead())
    {
        return;
    }

    bWantsToRun = false;
}

void ASTUPlayerCharacter::TryToJump()
{
    if (IsIdle() || IsMovingForward())
    {
        Jump();
    }
}
