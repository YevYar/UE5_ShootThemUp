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
    if (HealthWidgetComponent && GetWorld() && GetWorld()->GetFirstPlayerController()
        && GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())
    {
        const auto DistanceToPlayer =
          FVector::Distance(GetActorLocation(),
                            GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation());

        HealthWidgetComponent->SetVisibility(DistanceToPlayer <= HealthBarVisibilityMaxDistance, true);
    }
}
