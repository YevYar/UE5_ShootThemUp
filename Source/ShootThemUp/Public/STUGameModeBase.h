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

        void Killed(const AController* KillerController, const AController* VictimController) const;

    private:
        FLinearColor GetTeamColorByTeamID(int32 TeamID) const;
        void         InitTeamsData() const;
        void         LogPlayersStatistics() const;
        void         RespawnPlayers();
        void         RespawnOnePlayer(AController* Controller);
        void         SetPlayerColor(AController* Controller) const;
        void         SpawnBots();
        void         StartRound();
        void         UpdateRoundTimer();

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        TSubclassOf<AAIController> AIControllerClass;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        TSubclassOf<APawn> AIPawnClass;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        FGameData GameData;

    private:
        int32        CurrentRound       = 0;
        int32        RemainingRoundTime = 0;
        FTimerHandle RoundTimerHandle;
};
