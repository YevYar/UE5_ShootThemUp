// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "CoreMinimal.h"
#include "STUHealthPercentDecorator.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUHealthPercentDecorator : public UBTDecorator
{
        GENERATED_BODY()

    public:
        USTUHealthPercentDecorator();

    protected:
        bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0001", ClampMax = "1.0"))
        float HealthPercentToLookForPickup = 0.6;
};
