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

	FORCEINLINE void SetDialogoueGroupID(int32 NewGroupID) {DialogoueGroupID = NewGroupID;};
	FORCEINLINE UAPInteractionBoxComponent* GetInteractionBox() {return InteractionTrigger;};

protected:
	UPROPERTY(EditAnywhere)
	UAPInteractionBoxComponent* InteractionTrigger;

	UPROPERTY(EditAnywhere)
	UCameraComponent* InteractionCamera;

	UPROPERTY(EditAnywhere)
	int32 DialogoueGroupID; // Conversation Index

	UPROPERTY(EditAnywhere)
	float InteractFrequency = 0.1f;

	FTimerHandle InteractTimerHandle;
};
