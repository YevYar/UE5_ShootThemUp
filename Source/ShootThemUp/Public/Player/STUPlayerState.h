// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

UCLASS() class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
        GENERATED_BODY()

    public:
        FLinearColor GetTeamColor() const;
        int32        GetTeamID() const noexcept;

        void SetTeamColor(const FLinearColor& NewColor);
        void SetTeamID(int32 NewID) noexcept;

        // Game statistics
        int32 AddDeath() noexcept;
        int32 AddKill() noexcept;

        int32 GetDeathsScore() const noexcept;
        int32 GetKillsScore() const noexcept;

    private:
        int32        TeamID = 0;  // No Team
        FLinearColor TeamColor;

        // Game statistics
        int32 DeathsScore = 0;
        int32 KillsScore  = 0;
};
