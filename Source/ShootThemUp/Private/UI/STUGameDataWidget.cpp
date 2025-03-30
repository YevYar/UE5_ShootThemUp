// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUGameDataWidget.h"

#include "Player/STUPlayerState.h"
#include "STUGameModeBase.h"
#include "STUUtilities.h"

int32 USTUGameDataWidget::GetCurrentRound() const
{
    const auto GameMode = GetGameMode();
    return GameMode ? GameMode->GetCurrentRound() : 0;
}

int32 USTUGameDataWidget::GetNumberOfRounds() const
{
    const auto GameMode = GetGameMode();
    return GameMode ? GameMode->GetNumberOfRounds() : 0;
}

int32 USTUGameDataWidget::GetRoundRemainingTime() const
{
    const auto GameMode = GetGameMode();
    return GameMode ? GameMode->GetRoundRemainingTime() : 0;
}

int32 USTUGameDataWidget::GetPlayerDeaths() const
{
    const auto PlayerState = GetPlayerState();
    return PlayerState ? PlayerState->GetDeathsScore() : 0;
}

int32 USTUGameDataWidget::GetPlayerKills() const
{
    const auto PlayerState = GetPlayerState();
    return PlayerState ? PlayerState->GetKillsScore() : 0;
}

ASTUGameModeBase* USTUGameDataWidget::GetGameMode() const
{
    return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ASTUPlayerState* USTUGameDataWidget::GetPlayerState() const
{
    return STUUtils::GetSTUPlayerState(GetOwningPlayer());
}
