// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/Trap/APTrapBase.h"
#include "APTrapBase_CrossBow.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AAPTrapBase_CrossBow : public AAPTrapBase
{
	GENERATED_BODY()
public:	
	AAPTrapBase_CrossBow();

protected:
	virtual void BeginPlay() override;

	void ReadyToShoot();
	void ShootArrow();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
	

protected:
	// Trap Operate Timer
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere)
	float TrapOperationTime = 3.0f;

	// Trap Operate Timer
	FTimerHandle ShootTimerHandle;

	UPROPERTY(EditAnywhere)
	float ShootDelayTime = 3.0f;
};
