 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APInteractionTriggerBase.generated.h"

class UBoxComponent;

UCLASS()
class ARCANEPUNK_API AAPInteractionTriggerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPInteractionTriggerBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnInteractionTrigger(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* InteractionTrigger;

};
