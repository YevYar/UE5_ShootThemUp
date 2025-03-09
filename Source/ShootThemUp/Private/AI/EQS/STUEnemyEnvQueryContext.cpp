// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/EQS/STUEnemyEnvQueryContext.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void USTUEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
    const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);

    if (Blackboard)
    {
        const auto EnemyActor = Cast<AActor>(Blackboard->GetValueAsObject(EnemyActorKeyName));
        UEnvQueryItemType_Actor::SetContextHelper(ContextData, EnemyActor);
    }
}
