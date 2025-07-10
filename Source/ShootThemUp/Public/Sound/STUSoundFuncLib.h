// "Shoot Them Up" Game by YevYar, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "STUSoundFuncLib.generated.h"

class USoundClass;

UCLASS() class SHOOTTHEMUP_API USTUSoundFuncLib : public UBlueprintFunctionLibrary
{
        GENERATED_BODY()

    public:
        UFUNCTION(BlueprintCallable, Category = "Sound")
        static void SetSoundClassVolume(USoundClass* SoundClass, float NewVolume);
        UFUNCTION(BlueprintCallable, Category = "Sound")
        static void ToggleSoundClassMute(USoundClass* SoundClass);
};
