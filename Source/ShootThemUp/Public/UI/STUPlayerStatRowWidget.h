// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUPlayerStatRowWidget.generated.h"

class UImage;
class UTextBlock;

struct FPlayerStatisticsData;

UCLASS() class SHOOTTHEMUP_API USTUPlayerStatRowWidget : public UUserWidget
{
        GENERATED_BODY()

    public:
        void SetPlayerStatistics(const FPlayerStatisticsData& PlayerStatistics, bool IsGamerStatistics = false);

    private:
        void SetTextBlockText(UTextBlock* TextBlock, const FText& Text);

    protected:
        UPROPERTY(meta = (BindWidget))
        UTextBlock* DeathsTextBlock;
        UPROPERTY(meta = (BindWidget))
        UTextBlock* KillsTextBlock;
        UPROPERTY(meta = (BindWidget))
        UTextBlock* PlayerNameTextBlock;
        UPROPERTY(meta = (BindWidget))
        UImage* PlayerIndicatorImage;
        UPROPERTY(meta = (BindWidget))
        UTextBlock* TeamTextBlock;
};
