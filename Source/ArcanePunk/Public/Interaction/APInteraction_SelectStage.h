// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/APInteractionTriggerBase.h"
#include "APInteraction_SelectStage.generated.h"

class UUserWidget;

UCLASS()
class ARCANEPUNK_API AAPInteraction_SelectStage : public AAPInteractionTriggerBase, public IInteractionInterface
{
	GENERATED_BODY()

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual FInteractData GetInteractData() override;
	virtual void Interact(AArcanePunkCharacter* PlayerCharacter) override;

private:

};
