// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/Components/STURespawnComponent.h"

USTURespawnComponent::USTURespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
    if (!GetWorld())
    {
        return;
    }

    RemainingTimeToRespawn = RespawnTime;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &USTURespawnComponent::UpdateRespawnTimer, 1.0f, true);
}

void USTURespawnComponent::UpdateRespawnTimer()
{
    if (--RemainingTimeToRespawn <= 0)
    {
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
        }

        ReadyToRespawn.Broadcast(Cast<AController>(GetOwner()));
    }
}
