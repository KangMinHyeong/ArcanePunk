// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APBuffComponent.generated.h"

class AArcanePunkCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPBuffComponent();

	FORCEINLINE float GetCurrentATKSpeedCoefficient() const {return CurrentATKSpeedCoefficient;}; 
	FORCEINLINE float GetCurrentATKCoefficient() const {return CurrentATKCoefficient;}; 

	// 계수 영구증가
	FORCEINLINE void AddATKSpeedCoefficient(float Add) {CurrentATKSpeedCoefficient = CurrentATKSpeedCoefficient + Add;}; 
	FORCEINLINE void AddATKCoefficient(float Add) {CurrentATKCoefficient = CurrentATKCoefficient + Add;}; 
	FORCEINLINE void AddMaxHPCoefficient(float Add) {CurrentMaxHPCoefficient = CurrentMaxHPCoefficient + Add;}; 
	FORCEINLINE void AddDEFCoefficient(float Add) {CurrentDEFCoefficient = CurrentDEFCoefficient + Add;}; 

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindBuffComp();
	UFUNCTION()
	void UpdateStatus();

	void ATKSpeedUp(float ATKSpeedCoefficient, bool Start);
	void ATKSpeedUp(float ATKSpeedCoefficient, float Time);
	void ATKUp(float ATKCoefficient, bool Start);
	void ATKUp(float ATKCoefficient, float Time);
	void MaxHPUp(float MaxHPCoefficient, bool Start);
	
private:
	void ATKSpeedUpEnd(FTimerHandle* TimerHandle, float ATKSpeedCoefficient);
	void ATKUpEnd(FTimerHandle* TimerHandle, float ATKCoefficient);

private:
	float CurrentATKSpeedCoefficient = 1.0f;
	float CurrentATKCoefficient = 1.0f;
	float CurrentMaxHPCoefficient = 1.0f;
	float CurrentCriticalPercentCoefficient = 1.0f;
	float CurrentCriticalDamageCoefficient = 1.0f;
	float CurrentDEFCoefficient = 1.0f;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
};
