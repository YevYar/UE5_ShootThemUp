// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUBaseWidget.generated.h"

class USoundCue;

UCLASS() class SHOOTTHEMUP_API USTUBaseWidget : public UUserWidget
{
        GENERATED_BODY()

    public:
        virtual void Show();

    protected:
        UPROPERTY(meta = (BindWidgetAnim), Transient)
        UWidgetAnimation* ShowAnimation;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
        USoundCue* ShowSound;
};
