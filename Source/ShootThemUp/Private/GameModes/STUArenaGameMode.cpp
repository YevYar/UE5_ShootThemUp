// "Shoot Them Up" Game by YevYar, All rights reserved


#include "GameModes/STUArenaGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerState.h"

AActor* ASTUArenaGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    const auto PlayerState = Player ? Player->GetPlayerState<ASTUPlayerState>() : nullptr;
    const auto TeamID      = PlayerState ? PlayerState->GetTeamID() : 1;

    if (!TeamPlayerStarts.Contains(TeamID))
    {
        TeamPlayerStarts.Add(TeamID, GetTeamPlayerStarts(TeamID));
    }

    const auto TeamStarts = TeamPlayerStarts[TeamID];

    if (TeamStarts.PlayerStarts.Num() > 0)
    {
        auto          Index       = 0;
        APlayerStart* PlayerStart = nullptr;

        do
        {
            Index       = FMath::RandRange(0, TeamStarts.PlayerStarts.Num() - 1);
            PlayerStart = TeamStarts.PlayerStarts[Index];
        } while (!IsSpawnPointFree(PlayerStart, TeamID));

        return TeamStarts.PlayerStarts[Index];
    }

    return Super::ChoosePlayerStart_Implementation(Player);
}

bool ASTUArenaGameMode::ShouldAddKill(const AController* KillerController, const ASTUPlayerState* KillerPlayerState,
                                      const AController*     VictimController,
                                      const ASTUPlayerState* VictimPlayerState) const
{
    return KillerController != VictimController && KillerPlayerState->GetTeamID() != VictimPlayerState->GetTeamID();
}

TArray<APlayerStart*> ASTUArenaGameMode::GetTeamPlayerStarts(int32 TeamID) const
{
    auto Result = TArray<APlayerStart*>{};

    if (!GetWorld())
    {
        return Result;
    }

    const auto TagToSearch = TeamID == 1 ? FName("Team1") : FName("Team2");

    for (auto It = TActorIterator<APlayerStart>{GetWorld()}; It; ++It)
    {
        auto Start = *It;
        if (Start && Start->PlayerStartTag == TagToSearch)
        {
            Result.Add(Start);
        }
    }

    return Result;
}

bool ASTUArenaGameMode::IsSpawnPointFree(AActor* SpawnPoint, int32 TeamID) const
{
    if (!SpawnPoint)
    {
        return false;
    }

    if (!GetWorld())
    {
        return false;
    }

    const auto Location       = SpawnPoint->GetActorLocation();
    const auto Sphere         = FCollisionShape::MakeSphere(50.0f);
    auto       Overlaps       = TArray<FOverlapResult>{};
    auto       QueryParams    = FCollisionQueryParams{};
    QueryParams.bTraceComplex = false;

    const auto bHasOverlap =
      GetWorld()->OverlapMultiByObjectType(Overlaps, Location, FQuat::Identity,
                                           FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), Sphere,
                                           QueryParams);

    if (Overlaps.Num() > 0)
    {
        for (const auto& Overlap : Overlaps)
        {
            const auto Player      = Cast<ASTUBaseCharacter>(Overlap.GetActor());
            const auto PlayerState = Player ? Player->GetPlayerState<ASTUPlayerState>() : nullptr;

            if (PlayerState && PlayerState->GetTeamID() != TeamID)
            {
                return true;  // Prevent cases when every teammate holds its own start point and enemy occupies the
                              // start point of the killed teammate
            }
        }
    }

    return !bHasOverlap;
}
