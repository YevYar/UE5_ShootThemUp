// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "STUGameModeBase.h"
#include "STULabyrinthGameMode.generated.h"

UCLASS() class SHOOTTHEMUP_API ASTULabyrinthGameMode : public ASTUGameModeBase
{
        GENERATED_BODY()

    private:
        bool ShouldAddKill(const AController* KillerController, const ASTUPlayerState* KillerPlayerState,
                           const AController*     VictimController,
                           const ASTUPlayerState* VictimPlayerState) const override;
};
