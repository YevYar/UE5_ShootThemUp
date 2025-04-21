// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Menu/UI/STUMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
    }
}

void USTUMenuWidget::OnStartGame()
{
    UGameplayStatics::OpenLevel(GetWorld(), StartLevelName);
}
