// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/Components/STUAIPerceptionComponent.h"

#include "AIController.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"

#include "Player/Components/STUHealthComponent.h"
#include "STUGameModeBase.h"
#include "STUUtilities.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
    {
        return nullptr;
    }

    const auto TeamsNumber = GameMode->GetTeamsNumber();

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller)
    {
        return nullptr;
    }

    auto PerceivedActors = TArray<AActor*>{};
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

    const auto IsAnyEnemySeen = [&TeamsNumber, &PerceivedActors, &Controller]()
    {
        for (const auto& PerceivedActor : PerceivedActors)
        {
            if (!PerceivedActor)
            {
                continue;
            }

            const auto PerceivedPawn = Cast<APawn>(PerceivedActor);
            if (PerceivedPawn && STUUtils::AreEnemies(TeamsNumber, Controller, PerceivedPawn->Controller))
            {
                return true;
            }
        }

        return false;
    }();

    if (!IsAnyEnemySeen)
    {
        GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PerceivedActors);
    }

    if (PerceivedActors.Num() == 0)
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

        const auto PerceivedPawn = Cast<APawn>(PerceivedActor);
        const auto AreEnemies    = PerceivedPawn
                                && STUUtils::AreEnemies(TeamsNumber, Controller, PerceivedPawn->Controller);

        const auto HealthComponent = PerceivedActor->FindComponentByClass<USTUHealthComponent>();
        if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
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
