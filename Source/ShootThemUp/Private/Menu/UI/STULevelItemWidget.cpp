// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Menu/UI/STULevelItemWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

const FLevelData& USTULevelItemWidget::GetLevelData() const noexcept
{
    return LevelData;
}

int8 USTULevelItemWidget::GetLevelId() const noexcept
{
    return LevelData.LevelId;
}

void USTULevelItemWidget::SetLevelData(FLevelData NewLevelData)
{
    LevelData = std::move(NewLevelData);

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(LevelData.LevelDisplayName));
    }
    if (LevelThumb)
    {
        LevelThumb->SetBrushFromTexture(LevelData.LevelThumb);
    }
}

void USTULevelItemWidget::SetSelection(bool Selected)
{
    if (LevelSelectionFrame)
    {
        LevelSelectionFrame->SetVisibility(Selected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void USTULevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnSelectButtonClick);
    }
}

void USTULevelItemWidget::OnSelectButtonClick()
{
    LevelSelected.Broadcast(GetLevelId());
}
