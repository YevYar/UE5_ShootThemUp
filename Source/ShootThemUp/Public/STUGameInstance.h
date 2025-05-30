// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "STUCoreTypes.h"

#include "STUGameInstance.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
        GENERATED_BODY()

    public:
        void Init() override;

        const TArray<FLevelData>& GetLevelsData() const noexcept;
        FName                     GetMainMenuLevelName() const;
        FLevelData                GetStartupLevelData() const;
        int8                      GetStartupLevelId() const noexcept;
        FName                     GetStartupLevelName() const;

        void SetStartupLevel(int8 LevelId);

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        TArray<FLevelData> LevelsData;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        FName MainMenuLevelName = NAME_None;

    private:
        int8 StartupLevelId = INVALID_LEVEL_ID;
};
