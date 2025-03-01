// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/STUAIController.h"

#include "AI/STUAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

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

    SetFocus(GetFocusOnActor());
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
