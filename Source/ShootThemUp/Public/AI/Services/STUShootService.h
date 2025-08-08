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
        void    AimAtTheHead(UBlackboardComponent* BlackboardComponent, const FVector& MuzzleLocation,
                             const FVector& EnemyActorLocation);
        bool    CanStartFire(const FVector& MuzzleForwardVector, const APawn* Pawn, const AActor* EnemyActor) const;
        FVector GetRandomLocationInTheRadiusOfTarget(const FVector& TargetLocation) const;
        void    SetRequiredRotationToShootFromLauncher(UBlackboardComponent* BlackboardComponent, const APawn* Pawn,
                                                       const AActor*              EnemyActor,
                                                       const USTUWeaponComponent* WeaponComponent) const;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        FBlackboardKeySelector EnemyActorKey;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0001", ClampMax = "1.0"))
        float HeadshotProbability = 0.4f;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        float MaximumDeviationFromTheTarget = 450.0f;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        FBlackboardKeySelector RotationToTargetKey;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        FBlackboardKeySelector UseRotationToTargetKey;
};
