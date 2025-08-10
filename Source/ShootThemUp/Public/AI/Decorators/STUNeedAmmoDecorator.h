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

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0001", ClampMax = "1.0"))
        float AmmoPercentToLookForPickup = 0.4f;
};
