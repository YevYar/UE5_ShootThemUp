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

    private:
        int32        TeamID = 0;  // No Team
        FLinearColor TeamColor;
};
