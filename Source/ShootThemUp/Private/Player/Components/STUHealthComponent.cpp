// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/Components/STUHealthComponent.h"

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

float USTUHealthComponent::GetHealth() const noexcept
{
    return Health;
}

void USTUHealthComponent::SetHealth(float NewHealth) noexcept
{
    if (NewHealth >= 0.0f && NewHealth <= MaxHealth)
    {
        Health = NewHealth;
    }
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;
}
