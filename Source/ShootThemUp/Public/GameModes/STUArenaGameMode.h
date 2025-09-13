// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "STUGameModeBase.h"

#include "STUArenaGameMode.generated.h"

class APlayerStart;

USTRUCT() struct FPlayerStartArrayWrapper
{
        GENERATED_BODY()

        FPlayerStartArrayWrapper() = default;

        FPlayerStartArrayWrapper(TArray<APlayerStart*> PS) : PlayerStarts(std::move(PS))
        {
        }

        UPROPERTY()
        TArray<APlayerStart*> PlayerStarts;
};

UCLASS() class SHOOTTHEMUP_API ASTUArenaGameMode : public ASTUGameModeBase
{
        GENERATED_BODY()

    public:
        AActor* ChoosePlayerStart_Implementation(AController* Player) override;

    private:
        bool ShouldAddKill(const AController* KillerController, const ASTUPlayerState* KillerPlayerState,
                           const AController*     VictimController,
                           const ASTUPlayerState* VictimPlayerState) const override;

        TArray<APlayerStart*> GetTeamPlayerStarts(int32 TeamID) const;
        bool                  IsSpawnPointFree(AActor* SpawnPoint, int32 TeamID) const;

    private:
        UPROPERTY()
        TMap<int32, FPlayerStartArrayWrapper> TeamPlayerStarts;
};
