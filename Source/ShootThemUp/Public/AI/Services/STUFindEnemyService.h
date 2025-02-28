// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"
#include "STUFindEnemyService.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUFindEnemyService : public UBTService
{
        GENERATED_BODY()

    public:
        USTUFindEnemyService();

    protected:
        void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        FBlackboardKeySelector EnemyActorKey;
};
