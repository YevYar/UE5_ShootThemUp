// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUGameInstance.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
        GENERATED_BODY()

    public:
        FName GetMainMenuLevelName() const;
        FName GetStartupLevelName() const;

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        FName MainMenuLevelName = NAME_None;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
        FName StartupLevelName = NAME_None;
};
