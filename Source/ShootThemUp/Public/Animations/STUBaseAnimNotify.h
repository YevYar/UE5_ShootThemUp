// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CoreMinimal.h"
#include "STUBaseAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

UCLASS() class SHOOTTHEMUP_API USTUBaseAnimNotify : public UAnimNotify
{
        GENERATED_BODY()

    public:
        void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                    const FAnimNotifyEventReference& EventReference) override;

    public:
        FOnNotifiedSignature OnNotified;
};
