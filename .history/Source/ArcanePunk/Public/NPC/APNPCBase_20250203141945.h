// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "APNPCBase.generated.h"

class AAPInteractionTriggerBase;

UCLASS()
class ARCANEPUNK_API AAPNPCBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAPNPCBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	
	FORCEINLINE FVector GetDestination() const {return DestinationComp->GetComponentLocation();};
	FORCEINLINE bool IsMovement() const {return bMovement;};
	FORCEINLINE void SetMovement(bool NewBool) {bMovement = NewBool;};
	FORCEINLINE bool ChangeMovement(bool ExistingValue) {bMovement = !ExistingValue; return bMovement;};
	FORCEINLINE bool IsStart() const {return bStart;};
	FORCEINLINE void SetStart(bool NewBool) {bStart = NewBool;};

	void InteractionCollisionEnable(bool NewBool);

private:
	void SpawnInteractionTrigger();

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* DestinationComp;

	UPROPERTY(EditAnywhere, Category = "Widget")
	UWidgetComponent* DialogueWidgetComp;
	
	bool bMovement = false;

	bool bStart = true;

	UPROPERTY(EditAnywhere)
	int32 NPCID;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAPInteractionTriggerBase> InteractionTrigger;

	TWeakObjectPtr<AAPInteractionTriggerBase> Interaction;

	TWeakObjectPtr<UAPTextWidgetComponent> DialogueUI;
	
};
