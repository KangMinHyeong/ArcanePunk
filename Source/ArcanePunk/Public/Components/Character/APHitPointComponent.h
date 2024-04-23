// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "APHitPointComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPHitPointComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAPHitPointComponent();
	void DistinctHitPoint(FVector ImpactPoint, AActor *HitActor);
	void SetCrowdControl(AActor *HitActor, ECharacterState UpdateState, float StateTime);

	FORCEINLINE void SetKnockBackVec(FVector NewValue) {KnockBackVec =  NewValue;};
	FORCEINLINE void SetKnockBackDist(float NewValue) {KnockBackDist =  NewValue;};
	FORCEINLINE void SetSlowPercent(uint8 NewValue) {SlowPercent =  NewValue;};

private:

	FVector KnockBackVec = FVector(0,0,0);

	float KnockBackDist = 0.0f;

	uint8 SlowPercent = 0;

};
