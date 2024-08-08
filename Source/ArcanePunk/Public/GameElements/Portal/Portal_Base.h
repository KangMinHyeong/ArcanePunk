// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "Portal_Base.generated.h"

class AArcanePunkCharacter;
class UAPInteractionBoxComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class ARCANEPUNK_API APortal_Base : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	APortal_Base();
	FORCEINLINE FName GetPortalID() const {return PortalID;};
	void InitHide(bool IsHidden);

protected:
	virtual void BeginPlay() override;

	virtual void StartTeleport(AArcanePunkCharacter* Character, FVector TeleportPoint);
	virtual void SpawnSound(FVector Location);

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PortalMesh;
	
	UPROPERTY(EditAnywhere)
	UAPInteractionBoxComponent* PortalInteractionTrigger;

	FTimerHandle Delay_TimerHandle;

	FTimerHandle InteractTimerHandle;

	UPROPERTY(EditAnywhere)
	float Delay_Time = 2.0f;

	UPROPERTY(EditAnywhere)
	float InteractFrequency = 0.05f;

	UPROPERTY(EditAnywhere)
	bool DefaultHidden = false;

	UPROPERTY(EditAnywhere)
	float SoundStartTime = 0.0f;

	UPROPERTY(EditAnywhere)
	FName PortalID;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PortalEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* PortalSound;

};
