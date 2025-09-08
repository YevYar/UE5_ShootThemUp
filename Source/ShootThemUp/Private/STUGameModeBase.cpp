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

APlayerController* ASTUGameModeBase::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
    auto PlayerController = Super::SpawnPlayerController(InRemoteRole, Options);

    if (PlayerController)
    {
        auto PlayerState = STUUtils::GetSTUPlayerState(PlayerController);

        if (PlayerState)
        {
            TeamIDIndex = FMath::RandRange(0, GameData.TeamsNumber - 1);
            InitBotData(PlayerState);
        }
    }

    return PlayerController;
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    InitPlayerData();
    SpawnBots();
    SubscribeOnBotsNotifiers();
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

int32 ASTUGameModeBase::GetTeamsNumber() const
{
    return GameData.TeamsNumber;
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
    if (KillerPlayerState && ShouldAddKill(KillerController, KillerPlayerState, VictimController, VictimPlayerState))
    {
        KillerPlayerState->AddKill();
    }
}

APawn* ASTUGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController*      NewPlayer,
                                                                    const FTransform& SpawnTransform)
{
    auto SpawnInfo                            = FActorSpawnParameters{};
    SpawnInfo.Instigator                      = GetInstigator();
    SpawnInfo.ObjectFlags                    |= RF_Transient;  // We never want to save default player pawns into a map
    SpawnInfo.SpawnCollisionHandlingOverride  = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    const auto PawnClass  = GetDefaultPawnClassForController(NewPlayer);
    auto       ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
    if (!ResultPawn)
    {
        UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"),
               *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
    }
    return ResultPawn;
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

bool ASTUGameModeBase::ShouldAddKill(const AController* KillerController, const ASTUPlayerState* KillerPlayerState,
                                     const AController*     VictimController,
                                     const ASTUPlayerState* VictimPlayerState) const
{
    return true;
}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld())
    {
        return;
    }

    for (auto i = int32{0}; i < GameData.PlayersAmount - 1; ++i)
    {
        auto SpawnParameters = FActorSpawnParameters{};
        SpawnParameters.SpawnCollisionHandlingOverride =
          ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        const auto AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnParameters);
        auto       PlayerState  = STUUtils::GetSTUPlayerState(AIController);

        if (!PlayerState)
        {
            RestartPlayer(AIController);
            continue;
        }

        InitBotData(PlayerState, i + 1);
        RestartPlayer(AIController);
        SetPlayerColor(AIController);
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
    const auto TeamIDIdx = TeamID - 1;
    if (TeamIDIdx >= 0 && TeamIDIdx < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamIDIdx];
    }

    UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for TeamID %i. Set to default %s"), TeamID,
           *GameData.TeamDefaultColor.ToString());
    return GameData.TeamDefaultColor;
}

void ASTUGameModeBase::InitBotData(ASTUPlayerState* PlayerState, int32 BotIndex)
{
    const auto TeamID = int32{TeamIDIndex + 1};
    PlayerState->SetTeamID(TeamID);
    PlayerState->SetTeamColor(GetTeamColorByTeamID(TeamID));
    PlayerState->SetPlayerName(FString::Printf(TEXT("Bot %d"), BotIndex));

    TeamIDIndex = ++TeamIDIndex % GameData.TeamsNumber;
}

void ASTUGameModeBase::InitPlayerData()
{
    if (GetWorld())
    {
        const auto PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            SetPlayerColor(PlayerController);

            auto PlayerState = STUUtils::GetSTUPlayerState(PlayerController);
            if (PlayerState)
            {
                PlayerState->SetPlayerName("Player");
            }
        }
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
