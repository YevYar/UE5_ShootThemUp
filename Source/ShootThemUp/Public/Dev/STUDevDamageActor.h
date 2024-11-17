// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDevDamageActor.generated.h"

class UDamageType;

UCLASS() class SHOOTTHEMUP_API ASTUDevDamageActor : public AActor
{
        GENERATED_BODY()

    public:
        ASTUDevDamageActor();

    public:
        virtual void Tick(float DeltaTime) override;

    protected:
        virtual void BeginPlay() override;

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        USceneComponent* SceneComponent;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
        FColor Color = FColor::Red;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
        float Radius = 300.0f;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
        float Damage = 0.1f;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
        TSubclassOf<UDamageType> DamageType;
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
        bool DoFullDamage = false;
};
