// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "STUAIController.generated.h"

UCLASS() class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
        GENERATED_BODY()

    protected:
        void OnPossess(APawn* InPawn) override;
};
