// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Animations/STUEquipChangeWeaponAnimNotify.h"

void USTUEquipChangeWeaponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
    OnNotified.Broadcast(MeshComp);

    Super::Notify(MeshComp, Animation, EventReference);
}
