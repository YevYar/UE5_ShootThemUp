// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "STUCoreTypes.h"

#include "STUPlayerController.generated.h"

class USTURespawnComponent;

UCLASS() class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
        GENERATED_BODY()

    public:
        ASTUPlayerController();

    protected:
        void BeginPlay() override;
        void SetupInputComponent() override;

    private:
        void OnMatchStateChanged(ESTUMatchState NewMatchState);
        void PauseGame();

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USTURespawnComponent* RespawnComponent;
};
