// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUMenuHUD.generated.h"

UCLASS() class SHOOTTHEMUP_API ASTUMenuHUD : public AHUD
{
        GENERATED_BODY()

    protected:
        void BeginPlay() override;

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        TSubclassOf<UUserWidget> MenuWidgetClass;
};
