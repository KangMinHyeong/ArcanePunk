
#include "Interaction/APInteraction_SelectStage.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"

void AAPInteraction_SelectStage::Interact(AArcanePunkCharacter *PlayerCharacter)
{
    Super::Interact(PlayerCharacter);
    if(!PlayerCharacter) return;

    auto PC = Cast<AArcanePunkPlayerController>(PlayerCharacter->GetController()); if(!PC) return;
    PC->OpenStageSelectingUI(this);

}