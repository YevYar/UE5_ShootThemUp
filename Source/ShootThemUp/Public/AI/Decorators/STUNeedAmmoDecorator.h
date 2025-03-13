// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "CoreMinimal.h"
#include "STUNeedAmmoDecorator.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUNeedAmmoDecorator : public UBTDecorator
{
        GENERATED_BODY()

    public:
        USTUNeedAmmoDecorator();

    protected:
        bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
