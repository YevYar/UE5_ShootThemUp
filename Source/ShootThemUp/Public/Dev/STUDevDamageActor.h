// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDevDamageActor.generated.h"

UCLASS() class SHOOTTHEMUP_API ASTUDevDamageActor : public AActor
{
        GENERATED_BODY()

    public:
        // Sets default values for this actor's properties
        ASTUDevDamageActor();

    public:
        // Called every frame
        virtual void Tick(float DeltaTime) override;

    protected:
        // Called when the game starts or when spawned
        virtual void BeginPlay() override;

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        USceneComponent* SceneComponent;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
        FColor Color = FColor::Red;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
        float Damage = 0.1f;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
        bool DoFullDamage = false;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
        float Radius = 300.0f;
};
