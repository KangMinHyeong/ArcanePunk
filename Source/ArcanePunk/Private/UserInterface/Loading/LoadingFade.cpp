
#include "UserInterface/Loading/LoadingFade.h"

#include "PlayerController/ArcanePunkPlayerController.h"

void ULoadingFade::OnEndedFade()
{
    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer());
    if(PC) PC->OnEndedFadeOut.Broadcast();
}