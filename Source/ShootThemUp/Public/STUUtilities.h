// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "STUUtilities.generated.h"

class FSTURadialDamageEvent : public FDamageEvent
{
};

UCLASS() class SHOOTTHEMUP_API USTUUtilities : public UBlueprintFunctionLibrary
{
        GENERATED_BODY()

    public:
        UFUNCTION(BlueprintCallable, Category = "Utilities")
        static bool ApplyRadialDamage(UWorld* World, float MinDamage, float MaxDamage, const FVector& Origin,
                                      float DamageRadius, const TArray<AActor*>& IgnoreActors,
                                      AActor* DamageCauser = nullptr, AController* InstigatedByController = nullptr,
                                      bool DoFullDamage = false);
};
