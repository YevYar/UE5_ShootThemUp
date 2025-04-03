// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUSpectatorWidget.h"

#include "Player/Components/STURespawnComponent.h"

bool USTUSpectatorWidget::GetRemainingTimeToRespawn(int32& RemainingTime) const
{
    const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<USTURespawnComponent>();

    if (RespawnComponent && RespawnComponent->IsRespawningInProgress())
    {
        RemainingTime = RespawnComponent->GetRemainingTimeToRespawn();
        return true;
    }

    return false;
}
