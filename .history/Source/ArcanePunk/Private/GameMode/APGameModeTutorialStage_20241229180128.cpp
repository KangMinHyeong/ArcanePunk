
#include "GameMode/APGameModeTutorialStage.h"

void AAPGameModeTutorialStage::StartPlay()
{
    Super::StartPlay();

    
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAPGameModeTutorialStage::StartTutorial, FirstDelay, false);
}

void AAPGameModeTutorialStage::StartTutorial()
{
}