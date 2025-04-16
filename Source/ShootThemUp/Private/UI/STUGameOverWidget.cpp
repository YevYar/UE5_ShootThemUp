// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUGameOverWidget.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

#include "Player/STUPlayerState.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.h"
#include "STUUtilities.h"
#include "UI/STUPlayerStatRowWidget.h"

void USTUGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->MatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
        }
    }

    if (RestartGameButton)
    {
        RestartGameButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnRestartGame);
    }
}

void USTUGameOverWidget::OnRestartGame()
{
    const auto CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    UGameplayStatics::OpenLevel(GetWorld(), FName{CurrentLevelName});
}

void USTUGameOverWidget::BuildPlayersStatistics()
{
    if (!GetWorld() || !PlayerStatBox)
    {
        return;
    }

    PlayerStatBox->ClearChildren();

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller  = It->Get();
        auto       PlayerState = STUUtils::GetSTUPlayerState(Controller);

        if (!PlayerState)
        {
            continue;
        }

        auto PlayerStatRow = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidget);
        if (!PlayerStatRow)
        {
            continue;
        }

        const auto PlayerStatistics = FPlayerStatisticsData{.Deaths = PlayerState->GetDeathsScore(),
                                                            .Kills  = PlayerState->GetKillsScore(),
                                                            .Name   = FText::FromString(PlayerState->GetPlayerName()),
                                                            .Team   = PlayerState->GetTeamID()};
        PlayerStatRow->SetPlayerStatistics(PlayerStatistics, Controller->IsPlayerController());

        PlayerStatBox->AddChild(PlayerStatRow);
    }
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState NewMatchState)
{
    if (NewMatchState == ESTUMatchState::GameOver)
    {
        BuildPlayersStatistics();
    }
}
