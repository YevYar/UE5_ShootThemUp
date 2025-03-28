// "Shoot Them Up" Game by YevYar, All rights reserved


#include "STUGameModeBase.h"

#include "AIController.h"

#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "Player/STUPlayerState.h"
#include "STUUtilities.h"
#include "UI/STUGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass      = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    PlayerStateClass      = ASTUPlayerState::StaticClass();
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
    InitTeamsData();
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

FLinearColor ASTUGameModeBase::GetTeamColorByTeamID(int32 TeamID) const
{
    const auto TeamIDIndex = TeamID - 1;
    if (TeamIDIndex >= 0 && TeamIDIndex < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamIDIndex];
    }

    UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for TeamID %i. Set to default %s"), TeamID,
           *GameData.TeamDefaultColor.ToString());
    return GameData.TeamDefaultColor;
}

void ASTUGameModeBase::InitTeamsData() const
{
    if (!GetWorld())
    {
        return;
    }

    auto TeamIDIndex = int32{0};

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller  = It->Get();
        auto       PlayerState = STUUtils::GetSTUPlayerState(Controller);

        if (!PlayerState)
        {
            continue;
        }

        const auto TeamID = int32{TeamIDIndex + 1};
        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(GetTeamColorByTeamID(TeamID));
        SetPlayerColor(Controller);

        TeamIDIndex = ++TeamIDIndex % GameData.TeamsNumber;
    }
}

void ASTUGameModeBase::RespawnPlayers()
{
    if (!GetWorld())
    {
        return;
    }

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        RespawnOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::RespawnOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller) const
{
    const auto PlayerState = STUUtils::GetSTUPlayerState(Controller);
    if (!PlayerState)
    {
        return;
    }

    auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if (Character)
    {
        Character->SetPlayerColor(PlayerState->GetTeamColor());
    }
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
            RespawnPlayers();
            StartRound();
        }
        else
        {
            UE_LOG(LogSTUGameModeBase, Display, TEXT("============ GAME OVER ============"))
        }
    }
}
