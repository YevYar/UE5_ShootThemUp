// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS() class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
        GENERATED_BODY()

    public:
        ASTUAICharacter(const FObjectInitializer& ObjectInitializer);

        void Tick(float DeltaTime) override;

    protected:
        void BeginPlay() override;
        void OnHealthChanged(float NewHealth, bool IsCausedByDamage, float LastDamage) override;

    private:
        void UpdateHealthBarVisibility();

    public:
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
        UBehaviorTree* BehaviorTreeAsset;

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UWidgetComponent* HealthWidgetComponent;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Appearance")
        float HealthBarVisibilityMaxDistance = 1500.0f;
};
