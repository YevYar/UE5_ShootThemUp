// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "STUCoreTypes.h"

#include "STULevelItemWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;

UCLASS() class SHOOTTHEMUP_API USTULevelItemWidget : public UUserWidget
{
        GENERATED_BODY()

    public:
        const FLevelData& GetLevelData() const noexcept;
        int8              GetLevelId() const noexcept;

        void SetLevelData(FLevelData LevelData);
        void SetSelection(bool Selected);

    protected:
        void NativeOnInitialized() override;

    private:
        UFUNCTION()
        void OnSelectButtonClick();

    public:
        FLevelSelectedSignature LevelSelected;

    protected:
        UPROPERTY(meta = (BindWidget))
        UTextBlock* LevelNameTextBlock;
        UPROPERTY(meta = (BindWidget))
        UButton* LevelSelectButton;
        UPROPERTY(meta = (BindWidget))
        UImage* LevelSelectionFrame;
        UPROPERTY(meta = (BindWidget))
        UImage* LevelThumb;

    private:
        FLevelData LevelData;
};
