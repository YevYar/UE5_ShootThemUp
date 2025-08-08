// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "STUAIController.generated.h"

class USTUAIPerceptionComponent;
class USTURespawnComponent;

UCLASS() class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
        GENERATED_BODY()

    public:
        ASTUAIController();

        void Tick(float DeltaTime) override;

    protected:
        void OnPossess(APawn* InPawn) override;

        UFUNCTION()
        void StopBehaviorTree();

    private:
        AActor* GetFocusOnActor() const;
        void    SmoothRotateWithMovementSpeed(const FRotator& TargetRotation, float DeltaTime);

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
        FName FocusOnKeyName = "EnemyActor";
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
        FName RotationToTargetKeyName = "RotationToTarget";
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
        FName UseRotationToTargetKeyName = "UseRotationToTarget";

        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USTURespawnComponent* RespawnComponent;
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USTUAIPerceptionComponent* STUPerceptionComponent;
};
