
#include "Interaction/APInteraction_SelectStage.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"

void AAPInteraction_SelectStage::OnInteractionTrigger(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;
    auto PC = Cast<AArcanePunkPlayerController>(Character->GetController()); if(!PC) return;

    PC->OpenStageSelectingUI();
}
