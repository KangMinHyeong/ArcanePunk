 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Common/APInteractionBoxComponent.h"
#include "GameFramework/Actor.h"
#include "APInteractionTriggerBase.generated.h"

class UCameraComponent;

UCLASS()
class ARCANEPUNK_API AAPInteractionTriggerBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AAPInteractionTriggerBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual FInteractData GetInteractData() override;

	virtual void Interact(AArcanePunkCharacter* PlayerCharacter) override;

	FORCEINLINE void SetPartnerName(FName Name) {ConversationPartnerName = Name;};
	FORCEINLINE UAPInteractionBoxComponent* GetInteractionBox() {return InteractionTrigger;};

protected:
	UPROPERTY(EditAnywhere)
	UAPInteractionBoxComponent* InteractionTrigger;

	UPROPERTY(EditAnywhere)
	UCameraComponent* InteractionCamera;

	UPROPERTY(EditAnywhere)
	FName ConversationPartnerName;

	UPROPERTY(EditAnywhere)
	float InteractFrequency = 0.1f;

	FTimerHandle InteractTimerHandle;
};
