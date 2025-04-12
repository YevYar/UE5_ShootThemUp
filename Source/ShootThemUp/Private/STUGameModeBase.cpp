// "Shoot Them Up" Game by YevYar, All rights reserved


#include "STUGameModeBase.h"

#include "AIController.h"
#include "EngineUtils.h"

#include "Player/Components/STURespawnComponent.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "Player/STUPlayerState.h"
#include "STUUtilities.h"
#include "UI/STUGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

namespace
{
constexpr inline int32 MinRoundTimeForRespawn = 10;
}

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass      = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    PlayerStateClass      = ASTUPlayerState::StaticClass();
    HUDClass              = ASTUGameHUD::StaticClass();
}

bool ASTUGameModeBase::ClearPause()
{
    const auto IsPauseCleared = Super::ClearPause();

    if (IsPauseCleared)
    {
        MatchStateChanged.Broadcast(ESTUMatchState::InProgress);
    }

    return IsPauseCleared;
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto IsPaused = Super::SetPause(PC, CanUnpauseDelegate);

    if (IsPaused)
    {
        MatchStateChanged.Broadcast(ESTUMatchState::Paused);
    }

    return IsPaused;
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    SubscribeOnBotsNotifiers();
    InitTeamsData();
    SetMatchState(ESTUMatchState::InProgress);
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

int32 ASTUGameModeBase::GetCurrentRound() const
{
    return CurrentRound;
}

int32 ASTUGameModeBase::GetNumberOfRounds() const
{
    return GameData.RoundAmount;
}

int32 ASTUGameModeBase::GetRoundRemainingTime() const
{
    return RemainingRoundTime;
}

void ASTUGameModeBase::Killed(const AController* KillerController, const AController* VictimController) const
{
    auto VictimPlayerState = STUUtils::GetSTUPlayerState(VictimController);
    if (VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }

    StartPlayerRespawn(VictimController);

    if (KillerController == VictimController)
    {
        return;
    }

    auto KillerPlayerState = STUUtils::GetSTUPlayerState(KillerController);
    if (KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }
}

void ASTUGameModeBase::GameOver()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("============ GAME OVER ============"));
    LogPlayersStatistics();

    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }

    SetMatchState(ESTUMatchState::GameOver);
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

void ASTUGameModeBase::LogPlayersStatistics() const
{
    if (!GetWorld())
    {
        return;
    }

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto PlayerState = STUUtils::GetSTUPlayerState(It->Get());
        if (PlayerState)
        {
            PlayerState->LogStatistics();
        }
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

void ASTUGameModeBase::SetMatchState(ESTUMatchState NewMatchState)
{
    if (MatchState == NewMatchState)
    {
        return;
    }

    MatchState = NewMatchState;
    MatchStateChanged.Broadcast(MatchState);
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

void ASTUGameModeBase::StartPlayerRespawn(const AController* Controller) const
{
    const auto RespawnAvailable = RemainingRoundTime > MinRoundTimeForRespawn + GameData.RespawnTime;
    if (!Controller || !RespawnAvailable)
    {
        return;
    }

    const auto RespawnComponent = Controller->FindComponentByClass<USTURespawnComponent>();
    if (RespawnComponent)
    {
        RespawnComponent->Respawn(GameData.RespawnTime);
    }
}

void ASTUGameModeBase::StartRound()
{
    ++CurrentRound;
    RemainingRoundTime = GameData.RoundDuration;
    GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &ASTUGameModeBase::UpdateRoundTimer, 1.0f, true);
}

void ASTUGameModeBase::SubscribeOnBotsNotifiers()
{
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto RespawnComponent = It->Get()->FindComponentByClass<USTURespawnComponent>();
        if (RespawnComponent)
        {
            RespawnComponent->ReadyToRespawn.AddUObject(this, &ASTUGameModeBase::RespawnOnePlayer);
        }
    }
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
            GameOver();
        }
    }
}
