// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Menu/UI/STUMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"

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

void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    if (Animation == LoadAnimation)
    {
        OpenLevel();
    }

    Super::OnAnimationFinished_Implementation(Animation);
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

    if (LoadAnimation && !IsAnimationPlaying(LoadAnimation))
    {
        PlayAnimation(LoadAnimation);
    }

    UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void USTUMenuWidget::OpenLevel()
{
    const auto STUGameInstance = GetWorld() ? GetWorld()->GetGameInstance<USTUGameInstance>() : nullptr;
    if (STUGameInstance)
    {
        UGameplayStatics::OpenLevel(GetWorld(), STUGameInstance->GetStartupLevelName());
    }
}
