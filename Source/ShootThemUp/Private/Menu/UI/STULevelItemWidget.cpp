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
    if (LevelThumb)
    {
        LevelThumb->SetColorAndOpacity(Selected ? FLinearColor::Red : FLinearColor::White);
    }
}

void USTULevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnSelectButtonClick);
        LevelSelectButton->OnHovered.AddDynamic(this, &USTULevelItemWidget::OnLevelHovered);
        LevelSelectButton->OnUnhovered.AddDynamic(this, &USTULevelItemWidget::OnLevelUnovered);
        OnLevelUnovered();
    }
}

void USTULevelItemWidget::OnLevelHovered()
{
    if (LevelSelectionFrame)
    {
        LevelSelectionFrame->SetVisibility(ESlateVisibility::Visible);
    }
}

void USTULevelItemWidget::OnLevelUnovered()
{
    if (LevelSelectionFrame)
    {
        LevelSelectionFrame->SetVisibility(ESlateVisibility::Hidden);
    }
}

void USTULevelItemWidget::OnSelectButtonClick()
{
    LevelSelected.Broadcast(GetLevelId());
}
