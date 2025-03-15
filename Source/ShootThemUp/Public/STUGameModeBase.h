// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "STUCoreTypes.h"

#include "STUGameModeBase.generated.h"

class AAIController;

UCLASS() class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
        GENERATED_BODY()

    public:
        ASTUGameModeBase();

        UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
        void    StartPlay() override;

        UFUNCTION(BlueprintCallable, Category = "Development")
        bool IsDebug() const noexcept;

    private:
        void SpawnBots();

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        TSubclassOf<AAIController> AIControllerClass;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        TSubclassOf<APawn> AIPawnClass;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        FGameData GameData;
};
