// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/STUAICharacter.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AI/Components/STUAIWeaponComponent.h"
#include "AI/STUAIController.h"
#include "Player/Components/STUHealthComponent.h"
#include "UI/STUHealthBarWidget.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI     = EAutoPossessAI::PlacedInWorld;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate                  = FRotator{0.0f, 200.0f, 0.0f};
    }

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    if (HealthWidgetComponent)
    {
        HealthWidgetComponent->SetupAttachment(GetRootComponent());
        HealthWidgetComponent->SetDrawAtDesiredSize(true);
        HealthWidgetComponent->SetWidgetClass(USTUHealthBarWidget::StaticClass());
        HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    }
}

void ASTUAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateHealthBarVisibility();
}

void ASTUAICharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthWidgetComponent);
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();

    if (HealthWidgetComponent)
    {
        HealthWidgetComponent->SetVisibility(false, true);
    }
}

void ASTUAICharacter::OnHealthChanged(float NewHealth, bool IsCausedByDamage, float LastDamage)
{
    Super::OnHealthChanged(NewHealth, IsCausedByDamage, LastDamage);

    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if (HealthBarWidget)
    {
        HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
    }
}

void ASTUAICharacter::UpdateHealthBarVisibility()
{
    if (!HealthComponent->IsDead() && HealthWidgetComponent && GetWorld() && GetWorld()->GetFirstPlayerController()
        && GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())
    {
        const auto AICharacterLocation = GetActorLocation();
        const auto PlayerLocation   = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
        const auto DistanceToPlayer = FVector::Distance(AICharacterLocation, PlayerLocation);

        if (DistanceToPlayer > HealthBarVisibilityMaxDistance)
        {
            HealthWidgetComponent->SetVisibility(false, true);
            return;
        }

        auto HitResult       = FHitResult{};
        auto CollisionParams = FCollisionQueryParams{};
        CollisionParams.AddIgnoredActor(this);

        const auto IsAnyObstacle = GetWorld()->LineTraceSingleByChannel(HitResult, AICharacterLocation, PlayerLocation,
                                                                        ECC_Visibility, CollisionParams);

        HealthWidgetComponent->SetVisibility(!IsAnyObstacle, true);
    }
}
