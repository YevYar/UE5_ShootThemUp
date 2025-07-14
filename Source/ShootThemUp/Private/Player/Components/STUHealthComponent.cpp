// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/Components/STUHealthComponent.h"

#include "Engine/TimerHandle.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

#include "Damage/STUFireDamageType.h"
#include "Damage/STUIceDamageType.h"
#include "Damage/STULandingDamageType.h"
#include "Damage/STURadialDamageType.h"
#include "Player/STUBaseCharacter.h"
#include "STUGameModeBase.h"

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

void USTUHealthComponent::SetHealth(float NewHealth, bool IsCausedByDamage, AController* DamageCauser,
                                    float LastDamage) noexcept
{
    const auto LastHealth = Health;
    Health                = FMath::Clamp(NewHealth, MIN_HEALTH, MaxHealth);

    if (Health != LastHealth)
    {
        HealthChanged.Broadcast(Health, IsCausedByDamage, LastDamage);

        if (Health <= MIN_HEALTH + 0.9f)
        {
            bIsDead = true;
            Killed(DamageCauser);
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
        Owner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
        Owner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
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
    if (!DamageType)
    {
        return;
    }

    if (DamageType->IsA<USTURadialDamageType>())
    {
        ApplyDamage(Damage, InstigatedBy);
        PlayCameraShakeEffect(CameraShakeOnDamageEffect);
        return;
    }
    if (DamageType->IsA<USTULandingDamageType>())
    {
        ApplyDamage(Damage, InstigatedBy);

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
}

void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
                                            FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName,
                                            FVector ShotFromDirection, const UDamageType* DamageType,
                                            AActor* DamageCauser)
{
    const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
    ApplyDamage(FinalDamage, InstigatedBy);
    PlayCameraShakeEffect(CameraShakeOnDamageEffect);
}

void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                             FVector Origin, const FHitResult& HitInfo, AController* InstigatedBy,
                                             AActor* DamageCauser)
{
    ApplyDamage(Damage, InstigatedBy);
    PlayCameraShakeEffect(CameraShakeOnDamageEffect);
}

void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
    UE_LOG(LogHealth, Display, TEXT("Received damage: %f"), Damage);

    if (Damage <= 0.0f || IsDead())
    {
        return;
    }

    SetHealth(Health - Damage, true, InstigatedBy, Damage);
}

float USTUHealthComponent::GetPointDamageModifier(const AActor* DamagedActor, const FName& BoneName)
{
    const auto DamagedCharacter = Cast<ACharacter>(DamagedActor);
    if (!DamagedCharacter || !DamagedCharacter->GetMesh() || !DamagedCharacter->GetMesh()->GetBodyInstance(BoneName))
    {
        return 1.0f;
    }

    const auto PhysicalMaterial = DamagedCharacter->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    return PhysicalMaterial && DamageModifiersMap.Contains(PhysicalMaterial) ? DamageModifiersMap[PhysicalMaterial]
                                                                             : 1.0f;
}

void USTUHealthComponent::Killed(const AController* KillerController) const
{
    if (!KillerController || !GetWorld())
    {
        return;
    }

    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        const auto Pawn             = Cast<APawn>(GetOwner());
        const auto VictimController = Pawn ? Pawn->Controller : nullptr;

        GameMode->Killed(KillerController, VictimController);
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

void USTUHealthComponent::StopHealing()
{
    if (AutoHealEnabled)
    {
        const auto World = GetWorld();
        if (World)
        {
            World->GetTimerManager().ClearTimer(HealTimer);
        }
    }
}
