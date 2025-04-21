// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUMenuWidget.generated.h"

class UButton;

UCLASS() class SHOOTTHEMUP_API USTUMenuWidget : public UUserWidget
{
        GENERATED_BODY()

    protected:
        void NativeOnInitialized() override;

    protected:
        UPROPERTY(meta = (BindWidget))
        UButton* StartGameButton;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Menu")
        FName StartLevelName = "TestLevel";

    private:
        UFUNCTION()
        void OnStartGame();
};
