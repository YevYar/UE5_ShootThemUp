// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/STUPlayerController.h"

#include "Player/Components/STURespawnComponent.h"

ASTUPlayerController::ASTUPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("Respawn Component");
}
