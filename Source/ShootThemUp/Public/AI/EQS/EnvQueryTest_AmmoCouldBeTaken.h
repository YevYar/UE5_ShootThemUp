// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"

#include "EnvQueryTest_AmmoCouldBeTaken.generated.h"

UCLASS() class SHOOTTHEMUP_API UEnvQueryTest_AmmoCouldBeTaken : public UEnvQueryTest
{
        GENERATED_BODY()

    public:
        UEnvQueryTest_AmmoCouldBeTaken(const FObjectInitializer& ObjectInitializer);

        void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
