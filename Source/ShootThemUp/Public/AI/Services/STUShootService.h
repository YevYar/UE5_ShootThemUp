// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"
#include "STUShootService.generated.h"

class UBlackboardComponent;
class USTUWeaponComponent;

UCLASS() class SHOOTTHEMUP_API USTUShootService : public UBTService
{
        GENERATED_BODY()

    public:
        USTUShootService();

    protected:
        void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    private:
        void    SetRequiredRotationToShootFromLauncher(UBlackboardComponent* BlackboardComponent, const APawn* Pawn,
                                                       const AActor*              EnemyActor,
                                                       const USTUWeaponComponent* WeaponComponent) const;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        FBlackboardKeySelector EnemyActorKey;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        FBlackboardKeySelector IsCurrentWeaponLauncherKey;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        FBlackboardKeySelector ProjectileLaunchRotationKey;
};
