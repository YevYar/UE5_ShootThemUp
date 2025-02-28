// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/Tasks/STUNextLocationTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

USTUNextLocationTask::USTUNextLocationTask() : Super()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard)
    {
        return EBTNodeResult::Failed;
    }

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
    {
        return EBTNodeResult::Failed;
    }

    const auto NavSystem = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSystem)
    {
        return EBTNodeResult::Failed;
    }

    auto Center = Pawn->GetActorLocation();
    if (ShouldFindLocationAroundOtherActor)
    {
        const auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor)
        {
            return EBTNodeResult::Failed;
        }

        Center = CenterActor->GetActorLocation();
    }

    auto NavLocation = FNavLocation{};
    if (NavSystem->GetRandomReachablePointInRadius(Center, Radius, NavLocation))
    {
        Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
