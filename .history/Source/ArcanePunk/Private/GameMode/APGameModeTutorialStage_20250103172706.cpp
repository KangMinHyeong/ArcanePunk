
#include "GameMode/APGameModeTutorialStage.h"

#include "UserInterface/Tutorial/APTutorialUI.h"

void AAPGameModeTutorialStage::StartTutorial(APlayerController* PC)
{
    PC->SetPause(true);

    auto TutorialUI = Cast<UAPTutorialUI>(CreateWidget(PC, TutorialUIClass)); if(!TutorialUI) return;
    TutorialUI->AddToViewport();

    OnDestroyTutorialDummy.AddDynamic(this, &AAPGameModeTutorialStage::SpawnTutorialDummy);
    OnDestroyTutorialDummy.AddDynamic(TutorialUI, &UAPTutorialUI::OnSwitchingGuide);
}

void AAPGameModeTutorialStage::SpawnTutorialDummy()
{
    
}