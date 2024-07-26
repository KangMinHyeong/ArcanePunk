// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "APFadeOutTriggerComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UAPFadeOutTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
public:	
	UAPFadeOutTriggerComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere)
	float FadeSpeed = 0.15f;
	UPROPERTY(EditAnywhere)
	float FadeOutLimit = 0.2f;

	UPROPERTY()
	TArray<AActor*> FadeOutActors;

	UPROPERTY()
	TArray<AActor*> FadeInActors;
};
