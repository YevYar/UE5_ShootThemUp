// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Menu/UI/STUMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
    }

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
    }
}

void USTUMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USTUMenuWidget::OnStartGame()
{
    if (!GetWorld())
    {
        return;
    }

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if (STUGameInstance && !STUGameInstance->GetStartupLevelName().IsNone())
    {
        UGameplayStatics::OpenLevel(GetWorld(), STUGameInstance->GetStartupLevelName());
    }
    else
    {
        UE_LOG(LogSTUMenuWidget, Error, TEXT("Level name is NONE."));
    }
}
