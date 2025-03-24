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

namespace STUUtils
{
bool AreEnemies(const AController* Controller1, const AController* Controller2);

template<typename ComponentType>
requires std::is_base_of_v<UActorComponent, ComponentType>
ComponentType* GetSTUPlayerComponent(APawn* PlayerPawn)
{
    if (!PlayerPawn)
    {
        return nullptr;
    }

    return Cast<ComponentType>(PlayerPawn->GetComponentByClass(ComponentType::StaticClass()));
}
}  // namespace STUUtils
