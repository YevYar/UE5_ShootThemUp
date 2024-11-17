// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "STUCharacterMovementComponent.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUCharacterMovementComponent : public UCharacterMovementComponent
{
        GENERATED_BODY()

    public:
        float GetMaxSpeed() const override;

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 1000.0f));
        float RunSpeed = 3000.0f;
};
