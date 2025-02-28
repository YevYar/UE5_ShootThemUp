// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"
#include "STUNextLocationTask.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUNextLocationTask : public UBTTaskNode
{
        GENERATED_BODY()

    public:
        USTUNextLocationTask();

        EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "1.0"))
        float Radius = 1000.0f;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        FBlackboardKeySelector AimLocationKey;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        bool ShouldFindLocationAroundOtherActor = false;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI",
                  meta = (EditCondition = "ShouldFindLocationAroundOtherActor"))
        FBlackboardKeySelector CenterActorKey;
};
