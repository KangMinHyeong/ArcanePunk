// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APSkillHubComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillHubComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPSkillHubComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PressQ(); // Q Press 시 호출
	void PressE(); // E Press 시 호출
	void PressShift(); // Shift Press 시 호출
	void PressSpace(); // SpaceBar Press 시 호출

	void SetSkillState(uint8 NewValue);
	void AddSkillState(uint8 NewValue);
	void RemoveSkillState(uint8 NewValue = 0);
	UFUNCTION(BlueprintPure)
	uint8 GetSkillState();
private:
	void SkillDetermine(uint8 First, uint8 Second);
	void PlayBasicSkill();
	void CastSkillNumber(uint8 SkillNumber);
	
private:
	// Q,E, shift, space 스킬 컴포넌트
	UPROPERTY()
	class UAPSkillBaseQ* QSkillComp;
	UPROPERTY()
	class UAPSkillBaseE* ESkillComp;
	UPROPERTY()
	class UAPSkillBaseShift* ShiftSkillComp;
	UPROPERTY()
	class UAPSkillBaseSpace* SpaceSkillComp;

	// 스킬 Base State
	UPROPERTY()
	uint8 SkillState = 0;

	UPROPERTY()
	TArray<uint8>SkillStateArr;

	//TimerHandle
	FTimerHandle SkillCancleTimerHandle;
};
