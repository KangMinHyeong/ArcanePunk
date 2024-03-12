
#include "Interaction/APInteraction_Conversation.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "NPC/APNPCBase.h"

void AAPInteraction_Conversation::BeginFocus()
{
    TWeakObjectPtr<AArcanePunkCharacter> Character = InteractionTrigger->Character; if(!Character.IsValid()) return;
	Character->ActivateInteractionSweep();

    GetWorld()->GetTimerManager().SetTimer(InteractTimerHandle, this, &AAPInteraction_Conversation::BeginFocus, InteractFrequency, true);


    // Test, plan to Delete
    auto NPC = Cast<AAPNPCBase>(GetOwner());
    if(NPC) {NPC->SetMovement(true); Destroy();}
}

void AAPInteraction_Conversation::EndFocus()
{
    GetWorld()->GetTimerManager().ClearTimer(InteractTimerHandle);
}

FInteractData AAPInteraction_Conversation::GetInteractData()
{
    return InteractionTrigger->GetInteractionData();
}

void AAPInteraction_Conversation::Interact(AArcanePunkCharacter *PlayerCharacter)
{
    if(!PlayerCharacter) return;

    auto PC = Cast<AArcanePunkPlayerController>(PlayerCharacter->GetController()); if(!PC) return;
    // To Do : ConversationPartnerState Upadte <- GameInstance?
    PC->OpenConversationUI(this, ConversationPartnerName, ConversationPartnerState);

    PlayerCharacter->SetInteraction(false);
    PlayerCharacter->InteractionActorRemove(this);
}