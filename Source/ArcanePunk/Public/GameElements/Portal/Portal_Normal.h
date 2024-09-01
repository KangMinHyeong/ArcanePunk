// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/Portal/Portal_Base.h"
#include "Portal_Normal.generated.h"

class UAPInteractionBoxComponent;
class AArcanePunkCharacter;
class AArcanePunkPlayerController;

UCLASS()
class ARCANEPUNK_API APortal_Normal : public APortal_Base
{
	GENERATED_BODY()
public:
	APortal_Normal();

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual FInteractData GetInteractData() override;
	virtual void Interact(AArcanePunkCharacter* PlayerCharacter) override;

	UFUNCTION()
	void OnEndedFadeOut();
	UFUNCTION()
	void OnStartFadeIn();
	
protected:	
	virtual void BeginPlay() override;

	virtual void StartTeleport(AArcanePunkCharacter* Character, FVector TeleportPoint) override;

	virtual void SpawnSound(FVector Location) override;

protected:
	UPROPERTY(EditAnywhere)
	bool BothSides = true;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Destination;

	UPROPERTY(EditAnywhere)
	UAPInteractionBoxComponent* DestinationTrigger;

	TWeakObjectPtr<AArcanePunkPlayerController> CharacterPC;

	FVector Dest;
};
