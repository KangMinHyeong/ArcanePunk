
#include "GameMode/APGameModeTutorialStage.h"

#include "UserInterface/Tutorial/APTutorialUI.h"

void AAPGameModeTutorialStage::StartTutorial(APlayerController* PC)
{
    PC->SetPause(true);
    
    auto TutorialUI = Cast<UAPTutorialUI>(CreateWidget(this, TutorialUIClass)); if(!TutorialUI) return;

}