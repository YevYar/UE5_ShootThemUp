// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/Services/STUChangeWeaponService.h"

#include "AIController.h"

#include "Weapons/Components/STUWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Pawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;

    if (Pawn)
    {
        auto WeaponComponent = Pawn->FindComponentByClass<USTUWeaponComponent>();
        if (WeaponComponent && FMath::FRand() <= Probability)
        {
            WeaponComponent->NextWeapon();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
