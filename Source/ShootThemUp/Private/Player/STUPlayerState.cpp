// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All);

FLinearColor ASTUPlayerState::GetTeamColor() const
{
    return TeamColor;
}

int32 ASTUPlayerState::GetTeamID() const noexcept
{
    return TeamID;
}

void ASTUPlayerState::SetTeamColor(const FLinearColor& NewColor)
{
    TeamColor = NewColor;
}

void ASTUPlayerState::SetTeamID(int32 NewID) noexcept
{
    if (NewID >= 0)
    {
        TeamID = NewID;
    }
}

int32 ASTUPlayerState::AddDeath() noexcept
{
    return ++DeathsScore;
}

int32 ASTUPlayerState::AddKill() noexcept
{
    return ++KillsScore;
}

int32 ASTUPlayerState::GetDeathsScore() const noexcept
{
    return DeathsScore;
}

int32 ASTUPlayerState::GetKillsScore() const noexcept
{
    return KillsScore;
}

void ASTUPlayerState::LogStatistics() const
{
    UE_LOG(LogSTUPlayerState, Display, TEXT("TeamID: %i;   Kills: %i;   Deaths: %i"), TeamID, KillsScore, DeathsScore);
}
