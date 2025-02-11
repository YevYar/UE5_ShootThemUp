// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/Components/STUHealthComponent.h"

#include "Engine/TimerHandle.h"
#include "GameFramework/Controller.h"

#include "Dev/STUFireDamageType.h"
#include "Dev/STUIceDamageType.h"
#include "Dev/STULandingDamageType.h"
#include "Player/STUBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealth, All, All)

namespace
{
constexpr inline float MIN_HEALTH = 0.0f;
}

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

float USTUHealthComponent::GetHealthPercent() const noexcept
{
    return MaxHealth > 0.0 ? GetHealth() / MaxHealth : 0.0f;
}

float USTUHealthComponent::GetHealth() const noexcept
{
    return Health;
}

bool USTUHealthComponent::IsDead() const noexcept
{
    return bIsDead;
}

bool USTUHealthComponent::IsHealthFull() const noexcept
{
    return Health == MaxHealth;
}

void USTUHealthComponent::SetHealth(float NewHealth, bool IsCausedByDamage, float LastDamage) noexcept
{
    const auto LastHealth = Health;
    Health                = FMath::Clamp(NewHealth, MIN_HEALTH, MaxHealth);

    if (Health != LastHealth)
    {
        HealthChanged.Broadcast(Health, IsCausedByDamage, LastDamage);

        if (Health == MIN_HEALTH)
        {
            bIsDead = true;
            Died.Broadcast();
            StopHealing();
            return;
        }

        // If we received any damage and auto healing is enabled
        if (LastHealth > Health && AutoHealEnabled)
        {
            StopHealing();

            auto World = GetWorld();
            if (World)
            {
                World->GetTimerManager().SetTimer(HealTimer, this, &USTUHealthComponent::AutoHeal, HealInterval, true,
                                                  HealDelay);
            }
        }
    }
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsDead() || IsHealthFull())
    {
        return false;
    }

    SetHealth(Health + HealthAmount);
    return true;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    bIsDead = false;
    Health  = MaxHealth;
    HealthChanged.Broadcast(Health, false, 0.0f);

    const auto Owner = GetOwner();
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::AutoHeal()
{
    SetHealth(Health + HealAmount);

    if (IsHealthFull())
    {
        StopHealing();
    }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                          AController* InstigatedBy, AActor* DamageCauser)
{
    UE_LOG(LogHealth, Display, TEXT("Received damage: %f"), Damage);

    if (Damage <= 0.0f || IsDead())
    {
        return;
    }

    SetHealth(Health - Damage, true, Damage);

    if (!DamageType)
    {
        return;
    }

    if (DamageType->IsA<USTULandingDamageType>())
    {
        if (Damage >= RequiredLandingDamageToShowLandingEffect)
        {
            UE_LOG(LogHealth, Display, TEXT("Hard landing!"));
            PlayCameraShakeEffect(CameraShakeOnLandingEffect);
            return;
        }
        
        UE_LOG(LogHealth, Display, TEXT("Small damage on landing!"));
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

    PlayCameraShakeEffect(CameraShakeOnDamageEffect);
}

void USTUHealthComponent::StopHealing()
{
    if (AutoHealEnabled)
    {
        auto World = GetWorld();
        if (World)
        {
            World->GetTimerManager().ClearTimer(HealTimer);
        }
    }
}

void USTUHealthComponent::PlayCameraShakeEffect(TSubclassOf<UCameraShakeBase> CameraShakeEffect)
{
    if (IsDead())
    {
        return;
    }

    const auto Pawn = Cast<APawn>(GetOwner());
    if (!Pawn)
    {
        return;
    }

    const auto Controller = Pawn->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager)
    {
        return;
    }

    Controller->PlayerCameraManager->StartCameraShake(CameraShakeEffect);
}
