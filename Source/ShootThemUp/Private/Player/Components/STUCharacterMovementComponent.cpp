// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/Components/STUCharacterMovementComponent.h"

#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const auto MaxSpeed  = Super::GetMaxSpeed();
    const auto Character = Cast<ASTUBaseCharacter>(GetPawnOwner());
    return Character && Character->IsRunning() ? RunSpeed : MaxSpeed;
}
