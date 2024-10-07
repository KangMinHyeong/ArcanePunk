// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/APInteractionTriggerBase.h"
#include "APInteraction_SelectStage.generated.h"

class UUserWidget;

UCLASS()
class ARCANEPUNK_API AAPInteraction_SelectStage : public AAPInteractionTriggerBase
{
	GENERATED_BODY()

public:	
	virtual void Interact(AArcanePunkCharacter* PlayerCharacter) override;

private:

};
