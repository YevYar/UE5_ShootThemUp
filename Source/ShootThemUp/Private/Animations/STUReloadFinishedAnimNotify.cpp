// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Animations/STUReloadFinishedAnimNotify.h"

void USTUReloadFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
    OnNotified.Broadcast(MeshComp);

    Super::Notify(MeshComp, Animation, EventReference);
}
