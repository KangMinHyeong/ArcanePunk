// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APCrowdControlComponent.generated.h"

UENUM()
enum class ECharacterState : uint8 // 추가할때마다 CC_priority 업데이트
{
    None        = 0,
    KnockBack   = 1,
    Stun 		= 2,
	Frozen		= 3,
    Sleep       = 4,
	Slow		= 5,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPCrowdControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPCrowdControlComponent();

protected:
	virtual void BeginPlay() override;

public:	
	FORCEINLINE float GetCurrentSlowCoefficient() const {return CurrentSlowCoefficient;}; 
	FORCEINLINE float GetCurrentFastCoefficient() const {return CurrentFastCoefficient;}; 
	
	FORCEINLINE void AddMoveSpeedCoefficient(float Add) {CurrentFastCoefficient = CurrentFastCoefficient + Add;}; 
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindCrowdComp();
	UFUNCTION()
	void UpdateStatus();

	// 플레이어 상태이상 함수
	void KnockBackState(FVector KnockBackPoint, float KnockBackDist, float KnockBackTime);
	void StunState(float StunTime);
	void FrozenState(float FrozenTime);
	void SleepState();
	void SlowState(int32 SlowPercent, float SlowTime);
	void BurnState(APawn* DamageCauser, float DOT, float TotalTime, float InRate);
	void BleedingState(APawn* DamageCauser, float BleedingDamage, float TotalTime, float InRate);
	void WeakState(float WeakCoefficient, float WeakTime);

	// 이속 Buff
	void FastState(float FastCoefficient, bool Start);
	void FastState(float FastCoefficient, float FastTime);

private:
	bool CalculateStateTime(ECharacterState UpdateState, FTimerHandle& StateTimerHandle,float StateTime);

	void OnCharacterState(ECharacterState UpdateState, bool IsStop);

	void ClearStopState();

	void SwitchingState(ECharacterState UpdateState);

	void NormalState();	

	void OnBurnDamage(APawn* DamageCauser, float DOT);
	void OnBleedingDamage(APawn* DamageCauser, float BleedingDamage);
		
	void KnockBackEnd();
	void StunEnd();
	void FrozenEnd();
	void SlowEnd(FTimerHandle* SlowTimerHandle, int32 SlowPercent);
	void BurnEnd();
	void BleedingEnd();
	void WeakEnd(FTimerHandle* WeakTimerHandle, float WeakCoefficient);

	// 이속 버프
	void FastEnd(FTimerHandle* FastTimerHandle, float FastCoefficient);
	
	float GetDefaultSpeed();
	float GetDefaultATK();
	void SetDefaultATK(float NewValue);
	
	void PlayStateEffect(ECharacterState UpdateState, bool IsPlay);

private:
	FTimerHandle KnockBackTimerHandle;

	FTimerHandle StunTimerHandle;

	FTimerHandle FrozenTimerHandle;

	FTimerHandle SleepTimerHandle;

	FTimerHandle BurnTimerHandle;
	FTimerHandle BurnEndTimerHandle;

	FTimerHandle BleedingTimerHandle;
	FTimerHandle BleedingEndTimerHandle;

	TMap<ECharacterState, float> StateMap;
	
	TArray<bool> CC_Priority;
	TArray<uint8> SlowPriority;

	float DefaultSlip = 0.0f;
	float DefaultSpeed = 0.0f;

	float CurrentSlowCoefficient = 1.0f;
	float CurrentFastCoefficient = 1.0f;
};
