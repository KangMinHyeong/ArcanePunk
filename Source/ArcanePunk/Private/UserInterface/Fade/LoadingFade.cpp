
#include "UserInterface/Fade/LoadingFade.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "GameInstance/APGameInstance.h"

void ULoadingFade::OnEndedFade()
{
    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); 
    if(APGI) APGI->OnEndedFadeOut.Broadcast();
}