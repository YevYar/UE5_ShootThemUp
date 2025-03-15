// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/EQS/EnvQueryTest_PickupCouldBeTaken.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"

#include "Pickups/STUBasePickup.h"

UEnvQueryTest_PickupCouldBeTaken::UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    Cost          = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* DataOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);
    const auto WantsToBeTaken = BoolValue.GetValue();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const auto Pickup = Cast<ASTUBasePickup>(GetItemActor(QueryInstance, It));
        if (!Pickup)
        {
            continue;
        }

        It.SetScore(TestPurpose, FilterType, Pickup->CouldBeTaken(), WantsToBeTaken);
    }
}
