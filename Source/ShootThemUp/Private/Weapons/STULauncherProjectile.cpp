// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Weapons/STULauncherProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "STUUtilities.h"
#include "Weapons/Components/STUWeaponVFXComponent.h"

ASTULauncherProjectile::ASTULauncherProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    SphereComponent   = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    VFXComponent      = CreateDefaultSubobject<USTUWeaponVFXComponent>("VFXComponent");

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
        SphereComponent->bReturnMaterialOnMove = true;
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
    check(VFXComponent);

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;

    SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
    SphereComponent->OnComponentHit.AddDynamic(this, &ASTULauncherProjectile::OnHit);

    SetLifeSpan(LifeSpan);
    SpawnTraceSmokeEffect();
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
    VFXComponent->PlayImpactVFX(Hit);

    GetRootComponent()->SetVisibility(false, true);
    if (SpawnedTraceSmokeEffect)
    {
        SpawnedTraceSmokeEffect->SetVisibility(true, true);
    }
    SetLifeSpan(LifeSpanAfterHit);
}

AController* ASTULauncherProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}

void ASTULauncherProjectile::SpawnTraceSmokeEffect()
{
    SpawnedTraceSmokeEffect =
      UNiagaraFunctionLibrary::SpawnSystemAttached(TraceSmokeEffect, GetRootComponent(), NAME_None, FVector::ZeroVector,
                                                   FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
}
