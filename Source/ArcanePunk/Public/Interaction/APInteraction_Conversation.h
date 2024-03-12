// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/APInteractionTriggerBase.h"
#include "APInteraction_Conversation.generated.h"

UCLASS()
class ARCANEPUNK_API AAPInteraction_Conversation : public AAPInteractionTriggerBase, public IInteractionInterface
{
	GENERATED_BODY()
public:	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual FInteractData GetInteractData() override;
	virtual void Interact(AArcanePunkCharacter* PlayerCharacter) override;

private:
	UPROPERTY(EditAnywhere)
	uint8 ConversationPartnerState;
};
