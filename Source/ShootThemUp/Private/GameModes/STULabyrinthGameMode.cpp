// "Shoot Them Up" Game by YevYar, All rights reserved


#include "GameModes/STULabyrinthGameMode.h"

bool ASTULabyrinthGameMode::ShouldAddKill(const AController* KillerController, const ASTUPlayerState* KillerPlayerState,
                                          const AController*     VictimController,
                                          const ASTUPlayerState* VictimPlayerState) const
{
    return KillerController != VictimController;
}
