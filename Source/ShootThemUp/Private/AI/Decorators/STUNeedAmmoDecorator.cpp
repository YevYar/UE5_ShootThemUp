// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/Decorators/STUNeedAmmoDecorator.h"

#include "AIController.h"

#include "AI/Components/STUAIWeaponComponent.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
    NodeName = "Need Ammo Pickup For Current Weapon";
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        return false;
    }

    const auto WeaponComponent =
      Controller->GetPawn() ? Controller->GetPawn()->FindComponentByClass<USTUAIWeaponComponent>() : nullptr;

    return WeaponComponent && WeaponComponent->DoesCurrentWeaponNeedAmmo();
}
