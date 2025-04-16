// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUPauseWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void USTUPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ContinueGameButton)
    {
        ContinueGameButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnContinueGame);
    }
}

void USTUPauseWidget::OnContinueGame()
{
    auto GameMode = GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr;

    if (GameMode)
    {
        GameMode->ClearPause();
    }
}
