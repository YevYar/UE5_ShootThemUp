// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"
#include "STUShootService.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUShootService : public UBTService
{
        GENERATED_BODY()

    public:
        USTUShootService();

    protected:
        void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        FBlackboardKeySelector EnemyActorKey;
};
