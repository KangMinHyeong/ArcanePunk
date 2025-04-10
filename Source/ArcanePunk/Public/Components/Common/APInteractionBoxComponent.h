// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Interfaces/InteractionInterface.h"
#include "APInteractionBoxComponent.generated.h"

class AArcanePunkCharacter;
class UInteractionWidget;
class UWidgetComponent;

UCLASS()
class ARCANEPUNK_API UAPInteractionBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
public:	
	UAPInteractionBoxComponent();
	FInteractData GetInteractionData() const {return InteractionData;};
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnInteractionTrigger(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnInteractionTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void BeginInteract(AArcanePunkCharacter* PlayerCharacter);
	UFUNCTION()
	void EndInteract(AArcanePunkCharacter* PlayerCharacter);

	void InitInteractUI();

private:
	UPROPERTY(EditAnywhere)
	FInteractData InteractionData;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* InteractWidgetComp;

	TWeakObjectPtr<UInteractionWidget> InteractUI;

	bool bFirstInteract = true;
};
