
#include "GameMode/APGameModeTutorialStage.h"

void AAPGameModeTutorialStage::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);

    NewPlayer->SetPause();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAPGameModeTutorialStage::StartTutorial, FirstDelay, false);
}

void AAPGameModeTutorialStage::StartTutorial()
{
}