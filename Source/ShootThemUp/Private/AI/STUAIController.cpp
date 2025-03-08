// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/STUAIController.h"

#include "AI/STUAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Player/Components/STUAIPerceptionComponent.h"
#include "Player/Components/STUHealthComponent.h"

ASTUAIController::ASTUAIController()
{
    STUPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("Perception Component");
    if (STUPerceptionComponent)
    {
        SetPerceptionComponent(*STUPerceptionComponent);
    }
}

void ASTUAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto BlackboardComponent = GetBlackboardComponent();
    if (!BlackboardComponent)
    {
        return;
    }

    if (BlackboardComponent->GetValueAsBool(IsCurrentWeaponLauncherKeyName))
    {
        SetRotationIfLauncher(DeltaTime);
    }
    else
    {
        SetFocus(GetFocusOnActor());
    }
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
    if (STUCharacter)
    {
        RunBehaviorTree(STUCharacter->BehaviorTreeAsset);

        const auto HealthComponent = STUCharacter->FindComponentByClass<USTUHealthComponent>();
        if (HealthComponent)
        {
            HealthComponent->Died.AddDynamic(this, &ASTUAIController::StopBehaviorTree);
        }
    }
}

void ASTUAIController::StopBehaviorTree()
{
    BrainComponent->StopLogic("The character died.");
}

AActor* ASTUAIController::GetFocusOnActor() const
{
    const auto BlackboardComponent = GetBlackboardComponent();
    if (BlackboardComponent)
    {
        return Cast<AActor>(BlackboardComponent->GetValueAsObject(FocusOnKeyName));
    }

    return nullptr;
}

void ASTUAIController::SetRotationIfLauncher(float DeltaTime)
{
    const auto BlackboardComponent = GetBlackboardComponent();
    if (!BlackboardComponent)
    {
        return;
    }

    const auto ProjectileLaunchRotation = BlackboardComponent->GetValueAsRotator(ProjectileLaunchRotationKeyName);
    SmoothRotateWithMovementSpeed(ProjectileLaunchRotation, DeltaTime);
}

void ASTUAIController::SmoothRotateWithMovementSpeed(const FRotator& TargetRotation, float DeltaTime)
{
    const auto CurrentRotation = GetControlRotation();
    const auto OwnedCharacter  = Cast<ACharacter>(GetPawn());

    if (OwnedCharacter && OwnedCharacter->GetCharacterMovement())
    {
        const auto RotationSpeed = OwnedCharacter->GetCharacterMovement()->RotationRate.Yaw;

        // Interpolate the rotation based on the movement component's rotation rate
        auto NewRotation  = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
        NewRotation.Pitch = TargetRotation.Pitch;

        SetControlRotation(NewRotation);
    }
}
