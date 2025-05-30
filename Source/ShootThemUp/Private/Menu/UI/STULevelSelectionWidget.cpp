// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Menu/UI/STULevelSelectionWidget.h"

#include "Components/HorizontalBox.h"

#include "Menu/UI/STULevelItemWidget.h"
#include "STUGameInstance.h"
#include "STUUtilities.h"

namespace
{
inline USTUGameInstance* GetSTUGameInstance(const UWorld* World)
{
    return World ? World->GetGameInstance<USTUGameInstance>() : nullptr;
}
}  // namespace

void USTULevelSelectionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InitLevelItems();

    const auto STUGameInstance = GetSTUGameInstance(GetWorld());
    if (STUGameInstance)
    {
        OnLevelSelected(STUGameInstance->GetStartupLevelId());
    }
}

void USTULevelSelectionWidget::InitLevelItems()
{
    const auto STUGameInstance = GetSTUGameInstance(GetWorld());
    if (!STUGameInstance)
    {
        return;
    }

    if (!LevelsBox)
    {
        return;
    }
    LevelsBox->ClearChildren();

    const auto& LevelsData = STUGameInstance->GetLevelsData();
    for (const auto& LevelData : LevelsData)
    {
        const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (LevelItemWidget)
        {
            LevelItemWidget->SetLevelData(LevelData);
            LevelItemWidget->LevelSelected.AddUObject(this, &USTULevelSelectionWidget::OnLevelSelected);

            LevelsBox->AddChild(LevelItemWidget);
            LevelItemWidgets.Add(LevelItemWidget);
        }
    }
}

void USTULevelSelectionWidget::OnLevelSelected(int8 LevelId)
{
    const auto STUGameInstance = GetSTUGameInstance(GetWorld());
    if (!STUGameInstance)
    {
        return;
    }

    STUGameInstance->SetStartupLevel(LevelId);

    for (const auto LevelItem : LevelItemWidgets)
    {
        if (LevelItem)
        {
            LevelItem->SetSelection(LevelItem->GetLevelId() == LevelId);
        }
    }
}
