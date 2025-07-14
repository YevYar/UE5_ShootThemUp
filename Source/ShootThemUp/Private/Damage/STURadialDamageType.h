// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"
#include "STURadialDamageType.generated.h"

UCLASS() class USTURadialDamageType : public UDamageType
{
        GENERATED_BODY()
};

class FSTURadialDamageEvent : public FDamageEvent
{
    public:
        FSTURadialDamageEvent();
};
