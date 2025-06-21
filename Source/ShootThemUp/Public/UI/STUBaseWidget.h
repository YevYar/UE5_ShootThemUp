// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUBaseWidget.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUBaseWidget : public UUserWidget
{
        GENERATED_BODY()

    public:
        virtual void Show();

    protected:
        UPROPERTY(meta = (BindWidgetAnim), Transient)
        UWidgetAnimation* ShowAnimation;
};
