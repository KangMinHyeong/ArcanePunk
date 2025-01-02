
#include "GameMode/APGameModeTutorialStage.h"

#include "UserInterface/Tutorial/APTutorialUI.h"

void AAPGameModeTutorialStage::StartTutorial(APlayerController* PC)
{
    PC->SetPause(true);
    UE_LOG(LogTemp, Display, TEXT("Your message"));
    auto TutorialUI = Cast<UAPTutorialUI>(CreateWidget(PC, TutorialUIClass)); if(!TutorialUI) return;

}