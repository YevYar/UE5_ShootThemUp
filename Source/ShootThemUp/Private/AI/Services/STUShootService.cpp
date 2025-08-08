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
        BlackboardComponent->SetValueAsBool(UseRotationToTargetKey.SelectedKeyName, false);

        const auto EnemyActor      = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyActorKey.SelectedKeyName));
        const auto Pawn            = Controller->GetPawn();
        const auto WeaponComponent = Pawn ? Pawn->FindComponentByClass<USTUWeaponComponent>() : nullptr;

        if (EnemyActor && Pawn && WeaponComponent)
        {
            const auto IsCurrentWeaponLauncher = WeaponComponent->GetCurrentWeaponType() == EWeaponType::EWT_Launcher
                                                 && WeaponComponent->CanFire();
            BlackboardComponent->SetValueAsBool(UseRotationToTargetKey.SelectedKeyName, IsCurrentWeaponLauncher);

            if (IsCurrentWeaponLauncher)
            {
                SetRequiredRotationToShootFromLauncher(BlackboardComponent, Pawn, EnemyActor, WeaponComponent);
            }

            const auto MuzzleForwardVector =
              WeaponComponent->GetCurrentWeaponMuzzleTransform().GetRotation().GetForwardVector();

            if (CanStartFire(MuzzleForwardVector, Pawn, EnemyActor))
            {
                if (!IsCurrentWeaponLauncher)
                {
                    AimAtTheHead(BlackboardComponent, WeaponComponent->GetCurrentWeaponMuzzleLocation(),
                                 EnemyActor->GetActorLocation());
                }

                WeaponComponent->StartFire();
            }
        }
        else if (WeaponComponent)
        {
            WeaponComponent->StopFire();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

void USTUShootService::AimAtTheHead(UBlackboardComponent* BlackboardComponent, const FVector& MuzzleLocation,
                                    const FVector& EnemyActorLocation)
{
    auto UseRotationToTarget = false;

    if (FMath::FRand() <= HeadshotProbability)
    {
        UseRotationToTarget = true;

        const auto EnemyHeadLocation = [&EnemyActorLocation]()
        {
            auto HeadLocationTemp  = EnemyActorLocation;
            HeadLocationTemp.Z    += 63;
            return HeadLocationTemp;
        }();

        const auto MuzzleVectorToEnemyHead = (EnemyHeadLocation - MuzzleLocation).GetSafeNormal();
        BlackboardComponent->SetValueAsRotator(RotationToTargetKey.SelectedKeyName,
                                               MuzzleVectorToEnemyHead.ToOrientationRotator());
    }

    BlackboardComponent->SetValueAsBool(UseRotationToTargetKey.SelectedKeyName, UseRotationToTarget);
}

bool USTUShootService::CanStartFire(const FVector& MuzzleForwardVector, const APawn* Pawn,
                                    const AActor* EnemyActor) const
{
    const auto EnemyVectorToPawn = (Pawn->GetActorLocation() - EnemyActor->GetActorLocation()).GetSafeNormal();
    const auto AngleBetween =
      FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(MuzzleForwardVector, EnemyVectorToPawn)));

    return AngleBetween >= 165.0f && AngleBetween <= 200.0f;
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
        BlackboardComponent->SetValueAsRotator(RotationToTargetKey.SelectedKeyName,
                                               LaunchVelocity.ToOrientationRotator());
    }
}
