// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/STUAICharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AI/STUAIController.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    AutoPossessAI     = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate                  = FRotator{0.0f, 200.0f, 0.0f};
    }
}
