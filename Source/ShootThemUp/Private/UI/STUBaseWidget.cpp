// "Shoot Them Up" Game by YevYar, All rights reserved


#include "UI/STUBaseWidget.h"

void USTUBaseWidget::Show()
{
    if (ShowAnimation && !IsAnimationPlaying(ShowAnimation))
    {
        PlayAnimation(ShowAnimation);
    }
}
