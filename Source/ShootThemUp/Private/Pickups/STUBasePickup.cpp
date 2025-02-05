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

void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTUBasePickup::BeginPlay()
{
    Super::BeginPlay();

    check(SphereComponent);
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

void ASTUBasePickup::RespawnPickup()
{
    if (GetRootComponent())
    {
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

        FTimerHandle RespawnTimer;
        GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASTUBasePickup::RespawnPickup, RespawnInterval);
    }
}
