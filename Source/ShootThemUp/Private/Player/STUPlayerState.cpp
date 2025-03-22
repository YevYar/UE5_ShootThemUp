// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/STUPlayerState.h"

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
