// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/APInteractionTriggerBase.h"
#include "APInteraction_Conversation.generated.h"

UCLASS()
class ARCANEPUNK_API AAPInteraction_Conversation : public AAPInteractionTriggerBase
{
	GENERATED_BODY()
public:	

	virtual void Interact(AArcanePunkCharacter* PlayerCharacter) override;

private:

};
