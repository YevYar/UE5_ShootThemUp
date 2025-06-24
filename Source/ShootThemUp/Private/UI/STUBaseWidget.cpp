// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUBaseWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void USTUBaseWidget::Show()
{
    if (ShowAnimation && !IsAnimationPlaying(ShowAnimation))
    {
        PlayAnimation(ShowAnimation);
    }

    UGameplayStatics::PlaySound2D(GetWorld(), ShowSound);
}
