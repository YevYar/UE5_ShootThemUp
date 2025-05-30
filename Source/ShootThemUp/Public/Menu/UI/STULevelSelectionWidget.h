// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "STUCoreTypes.h"

#include "STULevelSelectionWidget.generated.h"

class UHorizontalBox;
class USTULevelItemWidget;

UCLASS() class SHOOTTHEMUP_API USTULevelSelectionWidget : public UUserWidget
{
        GENERATED_BODY()

    protected:
        void NativeOnInitialized() override;

    private:
        void InitLevelItems();
        void OnLevelSelected(int8 LevelId);

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Menu")
        TSubclassOf<UUserWidget> LevelItemWidgetClass;
        UPROPERTY(meta = (BindWidget))
        UHorizontalBox* LevelsBox;

    private:
        UPROPERTY()
        TArray<USTULevelItemWidget*> LevelItemWidgets;
};
