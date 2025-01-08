// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Animations/STUBaseAnimNotify.h"

namespace AnimUtilities
{
template<typename NotifyType>
requires std::is_base_of_v<USTUBaseAnimNotify, NotifyType>
static NotifyType* FindFirstAnimNotifyInAnimMontage(const UAnimMontage* AnimMontage)
{
    if (!AnimMontage)
    {
        return nullptr;
    }

    for (const auto& Notify : AnimMontage->Notifies)
    {
        if (auto CastedNotify = Cast<NotifyType>(Notify.Notify))
        {
            return CastedNotify;
        }
    }

    return nullptr;
}

}  // namespace AnimUtilities
