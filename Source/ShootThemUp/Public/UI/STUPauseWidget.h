// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUPauseWidget.generated.h"

class UButton;

UCLASS() class SHOOTTHEMUP_API USTUPauseWidget : public UUserWidget
{
        GENERATED_BODY()

    public:
        bool Initialize() override;

    protected:
        UPROPERTY(meta = (BindWidget))
        UButton* ContinueGameButton;

    private:
        UFUNCTION()
        void OnContinueGame();
};
