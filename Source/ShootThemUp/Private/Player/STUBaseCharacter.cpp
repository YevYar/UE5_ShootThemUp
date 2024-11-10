// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/STUBaseCharacter.h"

#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/SpectatorPawn.h"
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

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    HealthComponent->Died.AddDynamic(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->HealthChanged.AddDynamic(this, &ASTUBaseCharacter::OnHealthChanged);

    // First time HealthChanged is called by HealthComponent in its BeginPlay(), what called before
    // ASTUBaseCharacter::BeginPlay(). So here we must call it manually to init the displayed value by HealthTextComponent
    OnHealthChanged(HealthComponent->GetHealth());
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

bool ASTUBaseCharacter::IsDead() const
{
    return HealthComponent->IsDead();
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
    if (IsDead())
    {
        return;
    }

    AddMovementInput(GetActorForwardVector(), Amount);

    bIsIdleForward   = Amount == 0.0f;
    bIsMovingForward = Amount > NULL_EPSILON;
}

void ASTUBaseCharacter::MoveRight(float Amount)
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

void ASTUBaseCharacter::OnDeath()
{
    bIsIdleForward   = true;
    bIsIdleRight     = true;
    bIsMovingForward = false;
    bIsMovingRight   = false;
    bWantsToRun      = false;

    HealthTextComponent->SetVisibility(false, true);
    GetCharacterMovement()->DisableMovement();
    PlayAnimMontage(DeathMontage);
    SetLifeSpan(10.0f);  // 10s

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUBaseCharacter::OnHealthChanged(float NewHealth)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), HealthComponent->GetHealth())));
}

void ASTUBaseCharacter::StartRun()
{
    if (IsDead())
    {
        return;
    }

    bWantsToRun = true;
}

void ASTUBaseCharacter::StopRun()
{
    if (IsDead())
    {
        return;
    }

    bWantsToRun = false;
}

void ASTUBaseCharacter::TryToJump()
{
    if (IsIdle() || IsMovingForward())
    {
        Jump();
    }
}
