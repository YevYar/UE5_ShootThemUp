// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "STUAIController.generated.h"

class USTUAIPerceptionComponent;

UCLASS() class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
        GENERATED_BODY()

    public:
        ASTUAIController();

        void Tick(float DeltaTime) override;

    protected:
        void OnPossess(APawn* InPawn) override;

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
        FName FocusOnKeyName = "EnemyActor";

        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USTUAIPerceptionComponent* STUPerceptionComponent;

    private:
        AActor* GetFocusOnActor() const;
};
