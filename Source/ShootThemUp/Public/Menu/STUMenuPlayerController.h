// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUMenuPlayerController.generated.h"

UCLASS() class SHOOTTHEMUP_API ASTUMenuPlayerController : public APlayerController
{
        GENERATED_BODY()

    protected:
        void BeginPlay() override;
};
