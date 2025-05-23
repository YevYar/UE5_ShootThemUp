// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "STUEnemyEnvQueryContext.generated.h"

UCLASS() class SHOOTTHEMUP_API USTUEnemyEnvQueryContext : public UEnvQueryContext
{
        GENERATED_BODY()

    public:
        void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
        FName EnemyActorKeyName = "EnemyActor";
};
