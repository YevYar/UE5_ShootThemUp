// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUHealthBarWidget.generated.h"

class UProgressBar;

UCLASS() class SHOOTTHEMUP_API USTUHealthBarWidget : public UUserWidget
{
        GENERATED_BODY()

    public:
        void SetHealthPercent(float HealthPercent);

    protected:
        UPROPERTY(meta = (BindWidget))
        UProgressBar* HealthProgressBar;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        FLinearColor BadColor = FLinearColor::Red;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        FLinearColor GoodColor = FLinearColor::Green;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        float HealthPercentVisibilityThreshold = 0.8;
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        float PercentColorThreshold = 0.3;
};
