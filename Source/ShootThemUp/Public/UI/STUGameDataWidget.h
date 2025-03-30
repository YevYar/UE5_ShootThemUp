// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUGameDataWidget.generated.h"

class ASTUGameModeBase;
class ASTUPlayerState;

UCLASS() class SHOOTTHEMUP_API USTUGameDataWidget : public UUserWidget
{
        GENERATED_BODY()

    public:
        UFUNCTION(BlueprintCallable, Category = "Game")
        int32 GetCurrentRound() const;
        UFUNCTION(BlueprintCallable, Category = "Game")
        int32 GetNumberOfRounds() const;
        UFUNCTION(BlueprintCallable, Category = "Game")
        int32 GetRoundRemainingTime() const;

        UFUNCTION(BlueprintCallable, Category = "PlayerStatistics")
        int32 GetPlayerDeaths() const;
        UFUNCTION(BlueprintCallable, Category = "PlayerStatistics")
        int32 GetPlayerKills() const;

    private:
        ASTUGameModeBase* GetGameMode() const;
        ASTUPlayerState*  GetPlayerState() const;
};
