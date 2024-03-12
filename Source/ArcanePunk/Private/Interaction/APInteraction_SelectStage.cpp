
#include "Interaction/APInteraction_SelectStage.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"

void AAPInteraction_SelectStage::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAPInteraction_SelectStage::BeginFocus()
{
    TWeakObjectPtr<AArcanePunkCharacter> Character = InteractionTrigger->Character; if(!Character.IsValid()) return;
   	Character->ActivateInteractionSweep();

    GetWorld()->GetTimerManager().SetTimer(InteractTimerHandle, this, &AAPInteraction_SelectStage::BeginFocus, InteractFrequency, true);
}

void AAPInteraction_SelectStage::EndFocus()
{
    GetWorld()->GetTimerManager().ClearTimer(InteractTimerHandle);
}

FInteractData AAPInteraction_SelectStage::GetInteractData()
{
    return InteractionTrigger->GetInteractionData();
}

void AAPInteraction_SelectStage::Interact(AArcanePunkCharacter *PlayerCharacter)
{
    if(!PlayerCharacter) return;

    auto PC = Cast<AArcanePunkPlayerController>(PlayerCharacter->GetController()); if(!PC) return;
    PC->OpenStageSelectingUI(this);
    
    PlayerCharacter->SetInteraction(false);
    PlayerCharacter->InteractionActorRemove(this);
}