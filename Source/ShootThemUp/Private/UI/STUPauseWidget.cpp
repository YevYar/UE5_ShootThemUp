// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUPauseWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

bool USTUPauseWidget::Initialize()
{
    const auto IsInitialized = Super::Initialize();

    if (IsInitialized && ContinueGameButton)
    {
        ContinueGameButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnContinueGame);
    }

    return IsInitialized;
}

void USTUPauseWidget::OnContinueGame()
{
    auto GameMode = GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr;

    if (GameMode)
    {
        GameMode->ClearPause();
    }
}
