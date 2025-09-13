// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Player/STUBaseCharacter.h"

// #include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "Damage/STULandingDamageType.h"
#include "Player/Components/STUCharacterMovementComponent.h"
#include "Player/Components/STUHealthComponent.h"
#include "Weapons/Components/STUWeaponComponent.h"

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer
            .SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent     = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
    DamageTextComponent = CreateDefaultSubobject<UTextRenderComponent>("DamageTextComponent");
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    if (HealthTextComponent)
    {
        HealthTextComponent->SetupAttachment(GetRootComponent());
        HealthTextComponent->SetOwnerNoSee(true);
    }
    if (DamageTextComponent)
    {
        DamageTextComponent->SetupAttachment(HealthTextComponent);
        DamageTextComponent->SetOwnerNoSee(true);
    }

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

UAISense_Sight::EVisibilityResult
  ASTUBaseCharacter::CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation,
                                   int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested,
                                   float& OutSightStrength, int32* UserData,
                                   const FOnPendingVisibilityQueryProcessedDelegate* Delegate)
{
    static const auto NAME_AILineOfSight = FName{TEXT("TestPawnLineOfSight")};

    auto       HitResult   = FHitResult{};
    const auto MeshSockets = GetMesh()->GetAllSocketNames();

    for (const auto& Socket : MeshSockets)
    {
        const auto SocketLocation = GetMesh()->GetSocketLocation(Socket);
        const auto bHitSocket =
          GetWorld()->LineTraceSingleByObjectType(HitResult, Context.ObserverLocation, SocketLocation,
                                                  FCollisionObjectQueryParams{ECC_TO_BITFIELD(ECC_WorldStatic)
                                                                              | ECC_TO_BITFIELD(ECC_WorldDynamic)},
                                                  FCollisionQueryParams{NAME_AILineOfSight, true, Context.IgnoreActor});

        OutNumberOfLoSChecksPerformed++;

        if (bHitSocket == false || (HitResult.GetActor() && HitResult.GetActor()->IsOwnedBy(this)))
        {
            OutSeenLocation  = SocketLocation;
            OutSightStrength = 1;

            return UAISense_Sight::EVisibilityResult::Visible;
        }
    }

    const auto bHit =
      GetWorld()->LineTraceSingleByObjectType(HitResult, Context.ObserverLocation, GetActorLocation(),
                                              FCollisionObjectQueryParams{ECC_TO_BITFIELD(ECC_WorldStatic)
                                                                          | ECC_TO_BITFIELD(ECC_WorldDynamic)},
                                              FCollisionQueryParams{NAME_AILineOfSight, true, Context.IgnoreActor});

    OutNumberOfLoSChecksPerformed++;

    if (bHit == false || (HitResult.GetActor() && HitResult.GetActor()->IsOwnedBy(this)))
    {
        OutSeenLocation  = GetActorLocation();
        OutSightStrength = 1;

        return UAISense_Sight::EVisibilityResult::Visible;
    }

    OutSightStrength = 0;

    return UAISense_Sight::EVisibilityResult::NotVisible;
}

void ASTUBaseCharacter::Reset()
{
    ResetFields();
    Super::Reset();
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::TurnOff()
{
    ResetFields();
    Super::TurnOff();
}

bool ASTUBaseCharacter::IsDead() const
{
    return HealthComponent->IsDead();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
    {
        return 0.0f;
    }

    const auto NormalizedVelocity = GetVelocity().GetSafeNormal();
    const auto AngleBetween       = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), NormalizedVelocity));
    const auto CrossProduct       = FVector::CrossProduct(GetActorForwardVector(), NormalizedVelocity);
    const auto Direction          = FMath::Sign(CrossProduct.Z);
    const auto Degrees            = FMath::RadiansToDegrees(AngleBetween);

    return CrossProduct.IsZero() ? Degrees : Direction * Degrees;
}

bool ASTUBaseCharacter::IsJumping() const
{
    return GetCharacterMovement() && GetCharacterMovement()->IsFalling();
}

bool ASTUBaseCharacter::IsRunning() const
{
    return false;
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& PlayerColor)
{
    const auto Material = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (Material)
    {
        Material->SetVectorParameterValue(MaterialColorName, PlayerColor);
    }
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(DamageTextComponent);
    check(GetCharacterMovement());
    check(WeaponComponent);

    HealthComponent->Died.AddDynamic(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->HealthChanged.AddDynamic(this, &ASTUBaseCharacter::OnHealthChanged);

    // First time HealthChanged is called by HealthComponent in its BeginPlay(), what called before
    // ASTUBaseCharacter::BeginPlay(). So here we must call it manually to init the displayed value by HealthTextComponent
    OnHealthChanged(HealthComponent->GetHealth());

    DamageTextComponent->SetVisibility(false, true);

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnLanding);
}

void ASTUBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    ResetFields();
}

void ASTUBaseCharacter::OnDeath()
{
    ResetFields();

    HealthTextComponent->SetVisibility(false, true);
    GetCharacterMovement()->DisableMovement();
    // PlayAnimMontage(DeathMontage);

    SetLifeSpan(LifeSpanAfterDeath);

    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    }

    if (GetMesh())
    {
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        GetMesh()->SetSimulatePhysics(true);
    }

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ASTUBaseCharacter::OnHealthChanged(float NewHealth, bool IsCausedByDamage, float LastDamage)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NewHealth)));

    if (IsCausedByDamage && LastDamage > 0.0f)
    {
        DamageTextComponent->SetVisibility(true, true);
        DamageTextComponent->SetText(FText::FromString(FString::Printf(TEXT("-%.2f"), LastDamage)));

        auto World = GetWorld();
        if (World)
        {
            World->GetTimerManager().ClearTimer(DamageDisplayTimer);
            World->GetTimerManager().SetTimer(
              DamageDisplayTimer,
              [this]()
              {
                  DamageTextComponent->SetVisibility(false, true);
              },
              3.0f, false);
        }
    }
}

void ASTUBaseCharacter::OnLanding(const FHitResult& LandingHit)
{
    const auto LandingVelocityZ = -GetVelocity().Z;

    if (LandingVelocityZ < LandingDamageVelocity.X)
    {
        return;
    }

    const auto ReceivedLandingDamage =
      FMath::GetMappedRangeValueClamped(LandingDamageVelocity, LandingDamage, LandingVelocityZ);

    TakeDamage(ReceivedLandingDamage, FSTULandingDamageEvent{}, Controller, this);
}

void ASTUBaseCharacter::ResetFields()
{
    WeaponComponent->StopFireAndZoom();

    if (DamageDisplayTimer.IsValid())
    {
        GetWorldTimerManager().ClearTimer(DamageDisplayTimer);
    }
}
