// "Shoot Them Up" Game by YevYar, All rights reserved


#include "STUGameModeBase.h"

#include "AIController.h"

#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass      = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass              = ASTUGameHUD::StaticClass();
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    StartRound();
}

bool ASTUGameModeBase::IsDebug() const noexcept
{
#if STU_DEBUG
    return true;
#else
    return false;
#endif
}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld())
    {
        return;
    }

    for (auto i = int32{0}; i < GameData.PlayersAmount - 1; ++i)
    {
        auto SpawnParameters                           = FActorSpawnParameters{};
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnParameters);
        RestartPlayer(AIController);
    }
}

void ASTUGameModeBase::StartRound()
{
    ++CurrentRound;
    RemainingRoundTime = GameData.RoundDuration;
    GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &ASTUGameModeBase::UpdateRoundTimer, 1.0f, true);
}

void ASTUGameModeBase::UpdateRoundTimer()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i | Round: %i/%i"), RemainingRoundTime, CurrentRound,
           GameData.RoundAmount);

    if (--RemainingRoundTime <= 0)
    {
        GetWorldTimerManager().ClearTimer(RoundTimerHandle);

        if (CurrentRound < GameData.RoundAmount)
        {
            StartRound();
        }
        else
        {
            UE_LOG(LogSTUGameModeBase, Display, TEXT("============ GAME OVER ============"))
        }
    }
}
