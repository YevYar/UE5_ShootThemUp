// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Pickups/STUBasePickup.h"

#include "Components/SphereComponent.h"

ASTUBasePickup::ASTUBasePickup()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");

    if (SphereComponent)
    {
        SetRootComponent(SphereComponent);
        SphereComponent->InitSphereRadius(55.0f);
        SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        SphereComponent->SetNotifyRigidBodyCollision(false);
        SphereComponent->SetGenerateOverlapEvents(true);
        SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    }
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const auto Pawn = Cast<APawn>(OtherActor);
    if (Pawn && GivePickupTo(Pawn))
    {
        TakePickup();
    }
}

bool ASTUBasePickup::CouldBeTaken() const
{
    return !GetWorldTimerManager().IsTimerActive(RespawnTimer);
}

void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator{0.0f, RotationYaw, 0.0f});
}

void ASTUBasePickup::BeginPlay()
{
    Super::BeginPlay();

    check(SphereComponent);

    GenerateRotationYaw();
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

void ASTUBasePickup::RespawnPickup()
{
    if (GetRootComponent())
    {
        GenerateRotationYaw();
        SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
        GetRootComponent()->SetVisibility(true, true);
    }
}

void ASTUBasePickup::TakePickup()
{
    if (GetRootComponent())
    {
        SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
        GetRootComponent()->SetVisibility(false, true);
        GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASTUBasePickup::RespawnPickup, RespawnInterval);
    }
}

void ASTUBasePickup::GenerateRotationYaw()
{
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;
    RotationYaw          = FMath::RandRange(1.0f, 2.0f) * Direction;
}
