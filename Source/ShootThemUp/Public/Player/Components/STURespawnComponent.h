// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "STUCoreTypes.h"

#include "STURespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
        GENERATED_BODY()

    public:
        USTURespawnComponent();

        void Respawn(int32 RespawnTime);

    public:
        FReadyToRespawnSignature ReadyToRespawn;

    private:
        void UpdateRespawnTimer();

    private:
        int32        RemainingTimeToRespawn = 0;
        FTimerHandle RespawnTimer;
};
