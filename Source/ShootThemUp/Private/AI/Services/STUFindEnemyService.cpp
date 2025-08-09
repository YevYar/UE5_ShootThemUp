// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/Services/STUFindEnemyService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Player/Components/STUAIPerceptionComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
    NodeName = "Find Enemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    const auto Controller          = OwnerComp.GetAIOwner();

    if (BlackboardComponent && Controller)
    {
        const auto PerceptionComponent = Controller->FindComponentByClass<USTUAIPerceptionComponent>();
        if (PerceptionComponent)
        {
            const auto EnemyActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyActorKey.SelectedKeyName));
            BlackboardComponent->SetValueAsObject(EnemyActorKey.SelectedKeyName,
                                                  PerceptionComponent->GetClosestEnemy(EnemyActor));
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
