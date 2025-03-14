// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/EQS/EnvQueryTest_AmmoCouldBeTaken.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"

#include "AI/Components/STUAIWeaponComponent.h"
#include "Pickups/STUAmmoPickup.h"
#include "Weapons/STULauncherWeapon.h"
#include "Weapons/STURifleWeapon.h"

UEnvQueryTest_AmmoCouldBeTaken::UEnvQueryTest_AmmoCouldBeTaken(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    Cost          = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_AmmoCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
    const auto Pawn = Cast<APawn>(QueryInstance.Owner.Get());
    if (!Pawn)
    {
        return;
    }

    const auto WeaponComponent = Pawn->FindComponentByClass<USTUAIWeaponComponent>();
    if (!WeaponComponent)
    {
        return;
    }

    const auto CurrentWeaponType = WeaponComponent->GetCurrentWeaponType();
    if (CurrentWeaponType == EWeaponType::EWT_Unknown)
    {
        return;
    }

    const auto WeaponClassToPickup = WeaponComponent->GetCurrentWeaponType() == EWeaponType::EWT_Launcher
                                       ? ASTULauncherWeapon::StaticClass()
                                       : ASTURifleWeapon::StaticClass();

    UObject* DataOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);
    const auto WantsToBeTaken = BoolValue.GetValue();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const auto AmmoPickup = Cast<ASTUAmmoPickup>(GetItemActor(QueryInstance, It));
        if (!AmmoPickup)
        {
            continue;
        }

        It.SetScore(TestPurpose, FilterType,
                    AmmoPickup->GetWeaponClass() == WeaponClassToPickup
                      || AmmoPickup->GetWeaponClass()->IsChildOf(WeaponClassToPickup),
                    WantsToBeTaken);
    }
}
