// "Shoot Them Up" Game by YevYar, All rights reserved


#include "STUGameModeBase.h"

#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass      = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass              = ASTUGameHUD::StaticClass();
}

bool ASTUGameModeBase::IsDebug() const noexcept
{
#if STU_DEBUG
    return true;
#else
    return false;
#endif
}
