// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/STUPlayerController.h"

#include "Player/Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

ASTUPlayerController::ASTUPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("Respawn Component");
}

void ASTUPlayerController::BeginPlay()
{
    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->MatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);
        }
    }
}

void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindAction("PauseGame", EInputEvent::IE_Pressed, this, &ASTUPlayerController::PauseGame);
    }
}

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState NewMatchState)
{
    bShowMouseCursor = NewMatchState == ESTUMatchState::Paused;

    if (bShowMouseCursor)
    {
        SetInputMode(FInputModeUIOnly{});
    }
    else
    {
        SetInputMode(FInputModeGameOnly{});
    }
}

void ASTUPlayerController::PauseGame()
{
    auto GameMode = GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr;

    if (GameMode)
    {
        GameMode->SetPause(this);
    }
}
