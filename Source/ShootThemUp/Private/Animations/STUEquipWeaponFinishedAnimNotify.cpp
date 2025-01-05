// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Animations/STUEquipWeaponFinishedAnimNotify.h"

void USTUEquipWeaponFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               const FAnimNotifyEventReference& EventReference)
{
    OnNotified.Broadcast(MeshComp);

    Super::Notify(MeshComp, Animation, EventReference);
}
