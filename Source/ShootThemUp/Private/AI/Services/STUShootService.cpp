// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/Services/STUShootService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Weapons/Components/STUWeaponComponent.h"

USTUShootService::USTUShootService()
{
    NodeName = "Shoot";
}

void USTUShootService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    const auto Controller          = OwnerComp.GetAIOwner();

    if (BlackboardComponent && Controller)
    {
        const auto EnemyActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyActorKey.SelectedKeyName));
        const auto Pawn       = Controller->GetPawn();
        if (EnemyActor && Pawn)
        {
            const auto WeaponComponent = Pawn->FindComponentByClass<USTUWeaponComponent>();
            if (WeaponComponent)
            {
                EnemyActor ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
            }
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
