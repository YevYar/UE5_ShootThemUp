// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Menu/UI/STUMainMenuButton.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMainMenuButton, All, All);

void USTUMainMenuButton::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &USTUMainMenuButton::OnMainMenuClick);
    }
}

void USTUMainMenuButton::OnMainMenuClick()
{
    if (!GetWorld())
    {
        return;
    }

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if (STUGameInstance && !STUGameInstance->GetMainMenuLevelName().IsNone())
    {
        UGameplayStatics::OpenLevel(GetWorld(), STUGameInstance->GetMainMenuLevelName());
    }
    else
    {
        UE_LOG(LogSTUMainMenuButton, Error, TEXT("Main menu level name is NONE."));
    }
}
