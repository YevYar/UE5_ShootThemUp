// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/STUBaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

namespace
{
constexpr inline float NORMAL_WALK_SPEED = 600.0f;  // Normal walking speed (in Unreal units per second)
constexpr inline float SPRINTING_SPEED   = 3000.0f;  // Sprinting speed
}  // namespace

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter()
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
        PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASTUBaseCharacter::StartSprint);
        PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASTUBaseCharacter::StopSprint);
        PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    }
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount);

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = bWantsToRun && Amount > 0.0f ? SPRINTING_SPEED : NORMAL_WALK_SPEED;
    }
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::StartSprint()
{
    bWantsToRun = true;
}

void ASTUBaseCharacter::StopSprint()
{
    bWantsToRun = false;
}
