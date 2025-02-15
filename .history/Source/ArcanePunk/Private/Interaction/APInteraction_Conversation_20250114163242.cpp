
#include "Interaction/APInteraction_Conversation.h"

#include "Character/ArcanePunkCharacter.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "NPC/APNPCBase.h"

void AAPInteraction_Conversation::Interact(AArcanePunkCharacter *PlayerCharacter)
{
    Super::Interact(PlayerCharacter);
    if(!PlayerCharacter) return;

    auto PC = Cast<AArcanePunkPlayerController>(PlayerCharacter->GetController()); if(!PC) return;
    // To Do : ConversationPartnerState Upadte <- GameInstance?
    PC->OpenConversationUI(this, DialogoueGroupID);

}