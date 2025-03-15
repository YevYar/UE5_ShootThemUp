// "Shoot Them Up" Game by YevYar, All rights reserved


#include "AI/Services/STUShootService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

#include "STUCoreConstants.h"
#include "Weapons/Components/STUWeaponComponent.h"

USTUShootService::USTUShootService()
{
    NodeName = "Shoot";
}

void USTUShootService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    const auto Controller          = OwnerComp.GetAIOwner();

    if (BlackboardComponent && Controller)
    {
        const auto EnemyActor      = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyActorKey.SelectedKeyName));
        const auto Pawn            = Controller->GetPawn();
        const auto WeaponComponent = Pawn ? Pawn->FindComponentByClass<USTUWeaponComponent>() : nullptr;

        if (EnemyActor && Pawn && WeaponComponent)
        {
            const auto IsCurrentWeaponLauncher = WeaponComponent->GetCurrentWeaponType() == EWeaponType::EWT_Launcher
                                                 && WeaponComponent->CanFire();
            BlackboardComponent->SetValueAsBool(IsCurrentWeaponLauncherKey.SelectedKeyName, IsCurrentWeaponLauncher);

            if (IsCurrentWeaponLauncher)
            {
                SetRequiredRotationToShootFromLauncher(BlackboardComponent, Pawn, EnemyActor, WeaponComponent);
            }

            WeaponComponent->StartFire();
        }
        else if (WeaponComponent)
        {
            WeaponComponent->StopFire();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

FVector USTUShootService::GetRandomLocationInTheRadiusOfTarget(const FVector& TargetLocation) const
{
    auto       Result                      = FVector{TargetLocation};
    auto       DirectionComponentToChange  = FMath::RandBool() ? &Result.X : &Result.Y;
    const auto Sign                        = FMath::RandBool() ? 1 : -1;
    *DirectionComponentToChange           += Sign * FMath::RandRange(0.0f, MaximumDeviationFromTheTarget);
    return Result;
}

void USTUShootService::SetRequiredRotationToShootFromLauncher(UBlackboardComponent* BlackboardComponent,
                                                              const APawn* Pawn, const AActor* EnemyActor,
                                                              const USTUWeaponComponent* WeaponComponent) const
{
    if (!BlackboardComponent || !Pawn || !EnemyActor || !WeaponComponent)
    {
        return;
    }

    const auto StartLocation  = WeaponComponent->GetCurrentWeaponMuzzleLocation();
    const auto TargetLocation = GetRandomLocationInTheRadiusOfTarget(EnemyActor->GetActorLocation());
    const auto LaunchSpeed    = GameConfig::PROJECTILE_INITIAL_SPEED;
    auto       LaunchVelocity = FVector{};

    if (UGameplayStatics::SuggestProjectileVelocity(GetWorld(), LaunchVelocity, StartLocation, TargetLocation,
                                                    LaunchSpeed, false, 0, 0,
                                                    ESuggestProjVelocityTraceOption::DoNotTrace,
                                                    FCollisionResponseParams::DefaultResponseParam, {}, false))
    {
        BlackboardComponent->SetValueAsRotator(ProjectileLaunchRotationKey.SelectedKeyName,
                                               LaunchVelocity.ToOrientationRotator());
    }
}
