// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/Components/STUHealthComponent.h"

#include "Dev/STUFireDamageType.h"
#include "Dev/STUIceDamageType.h"
#include "Player/STUBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealth, All, All)

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

    const auto Owner = GetOwner();
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                          AController* InstigatedBy, AActor* DamageCauser)
{
    UE_LOG(LogHealth, Display, TEXT("Received damage: %f"), Damage);
    SetHealth(Health - Damage);

    if (!DamageType)
    {
        return;
    }

    if (DamageType->IsA<USTUIceDamageType>())
    {
        UE_LOG(LogHealth, Display, TEXT("Sooooo cooooooold!"));
    }
    else if (DamageType->IsA<USTUFireDamageType>())
    {
        UE_LOG(LogHealth, Display, TEXT("Sooooo hooooooot!"));
    }
    else
    {
        UE_LOG(LogHealth, Display, TEXT("Wow, unknown damage type!"));
    }
}
