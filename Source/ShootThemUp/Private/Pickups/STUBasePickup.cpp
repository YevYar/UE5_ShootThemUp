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

void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTUBasePickup::BeginPlay()
{
    Super::BeginPlay();
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    UE_LOG(LogTemp, Error, TEXT("Overlap"));
    Destroy();
}
