// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS() class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
        GENERATED_BODY()

    public:
        ASTUBasePickup();

    public:
        virtual void Tick(float DeltaTime) override;

        void NotifyActorBeginOverlap(AActor* OtherActor) override;

    protected:
        virtual void BeginPlay() override;

        virtual bool GivePickupTo(APawn* PlayerPawn);

        void RespawnPickup();
        void TakePickup();

    private:
        void GenerateRotationYaw();

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
        float RespawnInterval = 5.0f;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        USphereComponent* SphereComponent;

    private:
        float RotationYaw = 1.0f;
};
