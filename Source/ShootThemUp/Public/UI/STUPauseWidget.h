// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "UI/STUBaseWidget.h"

#include "STUPauseWidget.generated.h"

class UButton;

UCLASS() class SHOOTTHEMUP_API USTUPauseWidget : public USTUBaseWidget
{
        GENERATED_BODY()

    protected:
        void NativeOnInitialized() override;

    private:
        UFUNCTION()
        void OnContinueGame();

    protected:
        UPROPERTY(meta = (BindWidget))
        UButton* ContinueGameButton;
};
