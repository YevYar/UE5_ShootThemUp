// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "STUCoreTypes.h"

#include "STUGameOverWidget.generated.h"

class UButton;
class UVerticalBox;

UCLASS() class SHOOTTHEMUP_API USTUGameOverWidget : public UUserWidget
{
        GENERATED_BODY()

    protected:
        void NativeOnInitialized() override;

    private:
        UFUNCTION()
        void OnRestartGame();

        void BuildPlayersStatistics();
        void OnMatchStateChanged(ESTUMatchState NewMatchState);

    protected:
        UPROPERTY(meta = (BindWidget))
        UVerticalBox* PlayerStatBox;
        UPROPERTY(meta = (BindWidget))
        UButton* RestartGameButton;

        UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<UUserWidget> PlayerStatRowWidget;
};
