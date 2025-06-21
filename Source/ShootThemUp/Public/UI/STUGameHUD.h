// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "STUCoreTypes.h"

#include "STUGameHUD.generated.h"

class USTUBaseWidget;

UCLASS() class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
        GENERATED_BODY()

    public:
        void DrawHUD() override;

    protected:
        void BeginPlay() override;

    protected:
        UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<USTUBaseWidget> GameOverWidget;
        UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<USTUBaseWidget> PauseWidget;
        UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<USTUBaseWidget> PlayerHUDWidget;

    private:
        void DrawCrossline();
        void OnMatchStateChanged(ESTUMatchState NewMatchState);

    private:
        UPROPERTY()
        USTUBaseWidget* CurrentGameWidget = nullptr;
        UPROPERTY()
        TMap<ESTUMatchState, USTUBaseWidget*> GameWidgets;
};
