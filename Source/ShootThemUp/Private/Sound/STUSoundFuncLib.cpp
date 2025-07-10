// "Shoot Them Up" Game by YevYar, All rights reserved


#include "Sound/STUSoundFuncLib.h"

#include "Sound/SoundClass.h"

void USTUSoundFuncLib::SetSoundClassVolume(USoundClass* SoundClass, float NewVolume)
{
    if (SoundClass)
    {
        SoundClass->Properties.Volume = NewVolume;
    }
}

void USTUSoundFuncLib::ToggleSoundClassMute(USoundClass* SoundClass)
{
    if (SoundClass)
    {
        const auto NewVolume = SoundClass->Properties.Volume > 0.0f ? 0.0f : 1.0f;
        SetSoundClassVolume(SoundClass, NewVolume);
    }
}
