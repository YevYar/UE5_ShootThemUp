// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "UI/STUBaseWidget.h"

#include "STUMenuWidget.generated.h"

class UButton;
class USoundCue;

UCLASS() class SHOOTTHEMUP_API USTUMenuWidget : public USTUBaseWidget
{
        GENERATED_BODY()

    protected:
        void NativeOnInitialized() override;

    protected:
        void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

    private:
        UFUNCTION()
        void OnQuitGame();
        UFUNCTION()
        void OnStartGame();
        UFUNCTION()
        void OpenLevel();

    protected:
        UPROPERTY(meta = (BindWidgetAnim), Transient)
        UWidgetAnimation* LoadAnimation;
        UPROPERTY(meta = (BindWidget))
        UButton* QuitGameButton;
        UPROPERTY(meta = (BindWidget))
        UButton* StartGameButton;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
        USoundCue* StartGameSound;
};
