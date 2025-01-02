// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/STULauncherProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "STUUtilities.h"

ASTULauncherProjectile::ASTULauncherProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    SphereComponent   = CreateDefaultSubobject<USphereComponent>("SphereComponent");

    if (MovementComponent)
    {
        MovementComponent->InitialSpeed           = 1700.0f;
        MovementComponent->ProjectileGravityScale = 1.2f;
    }

    if (SphereComponent)
    {
        SetRootComponent(SphereComponent);
        SphereComponent->InitSphereRadius(5.0f);
        SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        SphereComponent->SetNotifyRigidBodyCollision(true);
        SphereComponent->SetGenerateOverlapEvents(false);
        SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    }
}

void ASTULauncherProjectile::SetShotDirection(const FVector& Direction)
{
    ShotDirection = Direction;
}

void ASTULauncherProjectile::BeginPlay()
{
    Super::BeginPlay();

    check(MovementComponent);
    check(SphereComponent);

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;

    SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
    SphereComponent->OnComponentHit.AddDynamic(this, &ASTULauncherProjectile::OnHit);

    SetLifeSpan(LifeSpan);
}

void ASTULauncherProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Error, TEXT("Hit something!"));

    if (!GetWorld())
    {
        return;
    }

    MovementComponent->StopMovementImmediately();

    auto IgnoreActors = TArray<AActor*>{};
    IgnoreActors.Add(this);

    USTUUtilities::ApplyRadialDamage(GetWorld(), MinDamage, MaxDamage, Hit.ImpactPoint, DamageRadius, IgnoreActors,
                                     GetOwner(), GetController(), DoFullDamage);

    DrawDebugSphere(GetWorld(), Hit.ImpactPoint, DamageRadius, 20, FColor::Red, false, 1.0f, 0.0f, 3.0f);

    Destroy();
}

AController* ASTULauncherProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
