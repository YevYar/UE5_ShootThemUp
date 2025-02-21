// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/STUAIController.h"

#include "AI/STUAICharacter.h"
#include "Player/Components/STUAIPerceptionComponent.h"

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

    if (STUPerceptionComponent)
    {
        const auto ClosestActor = STUPerceptionComponent->GetClosestEnemy();
        if (ClosestActor)
        {
            SetFocus(ClosestActor);
        }
    }
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
    if (STUCharacter)
    {
        RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
    }
}
