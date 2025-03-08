// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"
#include "STUChangeWeaponService.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUChangeWeaponService : public UBTService
{
        GENERATED_BODY()

    public:
        USTUChangeWeaponService();

    protected:
        void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0001", ClampMax = "1.0"))
        float Probability = 0.6f;
};
