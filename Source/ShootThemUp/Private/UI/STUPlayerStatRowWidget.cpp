// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUPlayerStatRowWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "STUCoreTypes.h"
#include "STUUtilities.h"

void USTUPlayerStatRowWidget::SetPlayerStatistics(const FPlayerStatisticsData& PlayerStatistics, bool IsGamerStatistics)
{
    SetTextBlockText(PlayerNameTextBlock, PlayerStatistics.Name);
    SetTextBlockText(KillsTextBlock, STUUtils::TextFromInt32(PlayerStatistics.Kills));
    SetTextBlockText(DeathsTextBlock, STUUtils::TextFromInt32(PlayerStatistics.Deaths));
    SetTextBlockText(TeamTextBlock, STUUtils::TextFromInt32(PlayerStatistics.Team));
    PlayerIndicatorImage->SetVisibility(IsGamerStatistics ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USTUPlayerStatRowWidget::SetTeamColor(const FLinearColor& TeamColor)
{
    if (TeamColorImage)
    {
        TeamColorImage->SetColorAndOpacity(TeamColor);
    }
}

void USTUPlayerStatRowWidget::SetTextBlockText(UTextBlock* TextBlock, const FText& Text)
{
    if (TextBlock)
    {
        TextBlock->SetText(Text);
    }
}
