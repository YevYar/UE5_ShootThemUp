// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/Decorators/STUHealthPercentDecorator.h"

#include "AIController.h"

#include "Player/Components/STUHealthComponent.h"

USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
    NodeName = "Health Percent";
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        return false;
    }

    const auto HealthComponent =
      Controller->GetPawn() ? Controller->GetPawn()->FindComponentByClass<USTUHealthComponent>() : nullptr;

    return HealthComponent && !HealthComponent->IsDead()
           && HealthComponent->GetHealthPercent() <= HealthPercentToLookForPickup;
}
