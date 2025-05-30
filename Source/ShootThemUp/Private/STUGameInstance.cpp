// "Shoot Them Up" Game by YevYar, All rights reserved


#include "STUGameInstance.h"

void USTUGameInstance::Init()
{
    Super::Init();

    checkf(LevelsData.Num() > 0, TEXT("Levels data must be not empty!"));

    auto i = int8{0};
    for (auto& LevelData : LevelsData)
    {
        checkf(!LevelData.LevelDisplayName.IsNone() && !LevelData.LevelName.IsNone(), TEXT("Level display name and "
                                                                                           "level name must be not "
                                                                                           "NONE!"));
        LevelData.LevelId = i++;

        if (LevelsData.ContainsByPredicate(
              [&](const FLevelData& LData)
              {
                  return LevelData.LevelId != LData.LevelId
                         && (LevelData.LevelDisplayName == LData.LevelDisplayName
                             || LevelData.LevelName == LData.LevelName);
              }))
        {
            checkf(false, TEXT("Level display name and level name must be unique!"));
        }
    }

    StartupLevelId = 0;
}

const TArray<FLevelData>& USTUGameInstance::GetLevelsData() const noexcept
{
    return LevelsData;
}

FName USTUGameInstance::GetMainMenuLevelName() const
{
    return MainMenuLevelName;
}

FLevelData USTUGameInstance::GetStartupLevelData() const
{
    return LevelsData[StartupLevelId];
}

int8 USTUGameInstance::GetStartupLevelId() const noexcept
{
    return StartupLevelId;
}

FName USTUGameInstance::GetStartupLevelName() const
{
    return LevelsData[StartupLevelId].LevelName;
}

void USTUGameInstance::SetStartupLevel(int8 LevelId)
{
    if (LevelId > INVALID_LEVEL_ID && LevelId < LevelsData.Num())
    {
        StartupLevelId = LevelId;
    }
    else
    {
        checkf(false, TEXT("Invalid level ID was passed!"));
    }
}
