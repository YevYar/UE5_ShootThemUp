// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "STUCoreTypes.h"

#include "STUGameModeBase.generated.h"

class AAIController;
class ASTUPlayerState;

UCLASS() class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
        GENERATED_BODY()

    public:
        ASTUGameModeBase();

        bool    ClearPause() override;
        UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
        bool    SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
        void    StartPlay() override;

        UFUNCTION(BlueprintCallable, Category = "Development")
        bool IsDebug() const noexcept;

        int32 GetCurrentRound() const;
        int32 GetNumberOfRounds() const;
        int32 GetRoundRemainingTime() const;
        int32 GetTeamsNumber() const;

        void Killed(const AController* KillerController, const AController* VictimController) const;

    private:
        virtual void RespawnOnePlayer(AController* Controller);
        virtual bool ShouldAddKill(const AController* KillerController, const ASTUPlayerState* KillerPlayerState,
                                   const AController* VictimController, const ASTUPlayerState* VictimPlayerState) const;
        virtual void SpawnBots();

        void         GameOver();
        FLinearColor GetTeamColorByTeamID(int32 TeamID) const;
        void         InitTeamsData() const;
        void         LogPlayersStatistics() const;
        void         RespawnPlayers();
        void         SetMatchState(ESTUMatchState NewMatchState);
        void         SetPlayerColor(AController* Controller) const;
        void         StartPlayerRespawn(const AController* Controller) const;
        void         StartRound();
        void         SubscribeOnBotsNotifiers();
        void         UpdateRoundTimer();

    public:
        FMatchStateChangedSignature MatchStateChanged;

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        TSubclassOf<AAIController> AIControllerClass;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        TSubclassOf<APawn> AIPawnClass;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        FGameData GameData;

    private:
        int32          CurrentRound       = 0;
        ESTUMatchState MatchState         = ESTUMatchState::WaitingToStart;
        int32          RemainingRoundTime = 0;
        FTimerHandle   RoundTimerHandle;
};
