// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APBuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPBuffComponent();

	FORCEINLINE float GetCurrentATKSpeedCoefficient() const {return CurrentATKSpeedCoefficient;}; 
	FORCEINLINE float GetCurrentATKCoefficient() const {return CurrentATKCoefficient;}; 
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ATKSpeedUp(float ATKSpeedCoefficient, bool Start);
	void ATKSpeedUp(float ATKSpeedCoefficient, float Time);
	void ATKUp(float ATKCoefficient, bool Start);
	void ATKUp(float ATKCoefficient, float Time);

private:
	void ATKSpeedUpEnd(FTimerHandle* TimerHandle, float ATKSpeedCoefficient);
	void ATKUpEnd(FTimerHandle* TimerHandle, float ATKCoefficient);

private:
	float CurrentATKSpeedCoefficient = 1.0f;
	float CurrentATKCoefficient = 1.0f;
};
