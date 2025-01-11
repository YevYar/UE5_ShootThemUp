// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();

    // DrawCrossline();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    const auto PlayerWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidget);
    if (PlayerWidget)
    {
        PlayerWidget->AddToViewport();
    }
}

void ASTUGameHUD::DrawCrossline()
{
    const auto Width  = Canvas->SizeX;
    const auto Height = Canvas->SizeY;

    const auto CenterX = Width / 2.0f;
    const auto CenterY = Height / 2.0f;

    DrawLine(CenterX - 7.5f, CenterY - 7.5f, CenterX + 7.5f, CenterY + 7.5f, FLinearColor::Green, 1.5f);
    DrawLine(CenterX - 7.5f, CenterY + 7.5f, CenterX + 7.5f, CenterY - 7.5f, FLinearColor::Green, 1.5f);
}
