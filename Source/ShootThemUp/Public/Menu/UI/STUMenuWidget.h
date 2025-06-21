// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "UI/STUBaseWidget.h"

#include "STUMenuWidget.generated.h"

class UButton;

UCLASS() class SHOOTTHEMUP_API USTUMenuWidget : public USTUBaseWidget
{
        GENERATED_BODY()

    protected:
        void NativeOnInitialized() override;

    protected:
        UPROPERTY(meta = (BindWidget))
        UButton* QuitGameButton;
        UPROPERTY(meta = (BindWidget))
        UButton* StartGameButton;

    private:
        UFUNCTION()
        void OnQuitGame();
        UFUNCTION()
        void OnStartGame();
};
