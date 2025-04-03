// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUSpectatorWidget.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUSpectatorWidget : public UUserWidget
{
        GENERATED_BODY()

    public:
        UFUNCTION(BlueprintCallable, Category = "Game")
        bool GetRemainingTimeToRespawn(int32& RemainingTime) const;
};
