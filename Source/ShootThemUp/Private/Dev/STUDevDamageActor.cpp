// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Dev/STUDevDamageActor.h"

#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASTUDevDamageActor::ASTUDevDamageActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    if (!SceneComponent)
    {
        return;
    }

    SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void ASTUDevDamageActor::BeginPlay()
{
    Super::BeginPlay();

    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 32, Color, true);
}

// Called every frame
void ASTUDevDamageActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr,
                                        DoFullDamage);
}
