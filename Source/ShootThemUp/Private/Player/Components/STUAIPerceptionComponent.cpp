// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/Components/STUAIPerceptionComponent.h"

#include "AIController.h"
#include "Perception/AISense_Sight.h"

#include "Player/Components/STUHealthComponent.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    auto PerceivedActors = TArray<AActor*>{};
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

    if (PerceivedActors.Num() == 0)
    {
        return nullptr;
    }

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller)
    {
        return nullptr;
    }

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
    {
        return nullptr;
    }

    auto       ClosestDistance = MAX_FLT;
    auto       ClosestActor    = static_cast<AActor*>(nullptr);
    const auto OwnLocation     = Pawn->GetActorLocation();

    for (const auto& PerceivedActor : PerceivedActors)
    {
        if (!PerceivedActor)
        {
            continue;
        }

        const auto HealthComponent = PerceivedActor->FindComponentByClass<USTUHealthComponent>();
        if (HealthComponent && !HealthComponent->IsDead())  // TODO: check if enemy or not
        {
            const auto DistanceToActor = (PerceivedActor->GetActorLocation() - OwnLocation).Size();
            if (DistanceToActor < ClosestDistance)
            {
                ClosestDistance = DistanceToActor;
                ClosestActor    = PerceivedActor;
            }
        }
    }

    return ClosestActor;
}
