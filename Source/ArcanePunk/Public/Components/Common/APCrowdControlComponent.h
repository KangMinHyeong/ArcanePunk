// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APCrowdControlComponent.generated.h"

UENUM()
enum class ECharacterState : uint8
{
    None        = 0,
    KnockBack   = 1,
    Stun 		= 2,
    Sleep       = 3,
	Slow		= 4,
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 플레이어 상태이상 함수
	void KnockBackState(FVector KnockBackPoint, float KnockBackDist, float KnockBackTime);//후에 인자 추가 (상태시간)
	void StunState(float StunTime);//후에 인자 추가 (상태시간)
	void SleepState();//후에 인자 추가 (상태시간)
	void SlowState(float SlowCoefficient, float SlowTime);

private:
	bool CalculateStateTime(ECharacterState UpdateState, FTimerHandle& StateTimerHandle,float StateTime);

	void OnPlayerState(ECharacterState UpdateState, bool IsStop);
	void OnEnemyState(ECharacterState UpdateState, bool IsStop);

	void ClearStopState();

	void SwitchingState(ECharacterState UpdateState);

	void NormalState();	

	void KnockBackEnd();
	void StunEnd();
	void SlowEnd();

	void PlayStateEffect(ECharacterState UpdateState, bool IsPlay);
	
private:
	FTimerHandle KnockBackTimerHandle;

	FTimerHandle StunTimerHandle;

	FTimerHandle SleepTimerHandle;

	FTimerHandle SlowTimerHandle;

	TMap<ECharacterState, float> StateMap;
	
	TArray<bool> CC_Priority;

	float DefaultSlip = 0.0f;
	float DefaultSpeed = 0.0f;
};
