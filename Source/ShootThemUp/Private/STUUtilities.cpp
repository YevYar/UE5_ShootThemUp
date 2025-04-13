// "Shoot Them Up" Game by YevYar, All rights reserved


#include "STUUtilities.h"

#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Player/STUPlayerState.h"

bool USTUUtilities::ApplyRadialDamage(UWorld* World, float MinDamage, float MaxDamage, const FVector& Origin,
                                      float DamageRadius, const TArray<AActor*>& IgnoreActors, AActor* DamageCauser,
                                      AController* InstigatedByController, bool DoFullDamage)
{
    if (!World)
    {
        return false;
    }

    // Set what actors to seek out from it's collision channel
    TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
    TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));

    // Class that the sphere should hit against and include in the OutActors array (Can be null)
    UClass* SeekClass = AActor::StaticClass();  // NULL;

    TArray<AActor*> OutActors;

    if (UKismetSystemLibrary::SphereOverlapActors(World, Origin, DamageRadius, TraceObjectTypes, SeekClass,
                                                  IgnoreActors, OutActors)
        && !OutActors.IsEmpty())
    {
        int32 NumberOfDamagedActors = 0;

        for (auto& Actor : OutActors)
        {
            const auto DamageAmount =
              DoFullDamage
                ? MaxDamage
                : FMath::GetMappedRangeValueClamped(FVector2D{20.0f, DamageRadius}, FVector2D{MaxDamage, MinDamage},
                                                    (Actor->GetActorLocation() - Origin).Length());

            Actor->TakeDamage(DamageAmount, FSTURadialDamageEvent{}, InstigatedByController, DamageCauser);
            ++NumberOfDamagedActors;
        }

        return NumberOfDamagedActors > 0;
    }

    return false;
}

bool STUUtils::AreEnemies(const AController* Controller1, const AController* Controller2)
{
    if (!Controller1 || !Controller2 || Controller1 == Controller2)
    {
        return false;
    }

    const auto PlayerState1 = GetSTUPlayerState(Controller1);
    const auto PlayerState2 = GetSTUPlayerState(Controller2);

    return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
}

ASTUPlayerState* STUUtils::GetSTUPlayerState(const AController* Controller)
{
    return Controller ? Cast<ASTUPlayerState>(Controller->PlayerState) : nullptr;
}

FText STUUtils::TextFromInt32(int32 Value)
{
    return FText::FromString(FString::FromInt(Value));
}
