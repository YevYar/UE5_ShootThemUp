// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUMainMenuButton.generated.h"

class UButton;

UCLASS() class SHOOTTHEMUP_API USTUMainMenuButton : public UUserWidget
{
        GENERATED_BODY()

    protected:
        void NativeOnInitialized() override;

    protected:
        UPROPERTY(meta = (BindWidget))
        UButton* MainMenuButton;

    private:
        UFUNCTION()
        void OnMainMenuClick();
};
