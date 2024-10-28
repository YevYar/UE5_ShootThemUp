// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUHealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
        GENERATED_BODY()

    public:
        // Sets default values for this component's properties
        USTUHealthComponent();

        float GetHealth() const noexcept;
        void  SetHealth(float NewHealth) noexcept;

    protected:
        // Called when the game starts
        virtual void BeginPlay() override;

    protected:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0"))
        float MaxHealth = 100.0f;

    private:
        float Health = 0.0f;
};
