// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Components/Character/SkillNumber/SkillNumber1.h"
#include "Components/Character/SkillNumber/SkillNumber2.h"
#include "Components/Character/SkillNumber/SkillNumber3.h"
#include "Components/Character/SkillNumber/SkillNumber4.h"
#include "Components/Character/SkillNumber/SkillNumber5.h"
#include "Components/Character/SkillNumber/SkillNumber6.h"
#include "Components/Character/SkillNumber/SkillNumber7.h"
#include "Components/Character/SkillNumber/SkillNumber8.h"
#include "Components/Character/SkillNumber/SkillNumber9.h"
#include "Components/Character/SkillNumber/SkillNumber10.h"
#include "Components/Character/SkillNumber/SkillNumber11.h"
#include "Components/Character/SkillNumber/SkillNumber12.h"
#include "Components/Character/SkillNumber/SkillNumber13.h"
#include "Components/Character/SkillNumber/SkillNumber14.h"
#include "Components/Character/SkillNumber/UltSkillNumber_1.h"
#include "Components/ActorComponent.h"
#include "APSkillHubComponent.generated.h"

class USkillNumberBase;
class AArcanePunkCharacter;

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

	// void SetSkillState(ESkillKey NewValue);
	// void AddSkillState(ESkillKey NewValue);
	// void RemoveSkillState(ESkillKey NewValue = ESkillKey::None);
	UFUNCTION(BlueprintPure)
	ESkillKey GetSkillState();

	// FORCEINLINE UAPSkillNumber* GetAPSkillNumberComponent() const {return SkillNumComp;}; // SkillNumComp 반환

	void UpdateSkill_Q();
	void UpdateSkill_E();
	void UpdateSkill_R();

	USkillNumberBase* GetSKillNumberComponent(ESkillNumber SkillNumber);

	void AutoRecoveryMP();

private:
	// void SkillDetermine(ESkillKey First, ESkillKey Second);
	void PlayBasicSkill();

	void RecoveryMP();
	// void CastSkillNumber(ESkillNumber SkillNumber, ESkillKey WhichKey);
	// void CastUltSkillNumber(EUltSkillNumber UltSkillNumber, ESkillKey WhichKey);
	// bool ActivationCheck(ESkillNumber SkillNumber);
	
private:
	// Q,E, shift, space 스킬 컴포넌트
	// UPROPERTY(EditAnywhere)
	// class UAPSkillBaseQ* QComp;
	// UPROPERTY(EditAnywhere)
	// class UAPSkillBaseE* EComp;
	// UPROPERTY(EditAnywhere)
	// class UAPSkillBaseSpace* SpaceComp;

	// 스킬 Base State
	ESkillKey SkillState = ESkillKey::None;

	TArray<ESkillKey>SkillStateArr;

	//TimerHandle
	FTimerHandle SkillCancleTimerHandle;
	FTimerHandle RecoveryMPTimerHandle;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	// UPROPERTY(EditAnywhere, Category = "Component")
	// UAPSkillNumber* SkillNumComp;
	bool Proceeding = false;

	UPROPERTY()
	float RecoveryTime_MP = 10.0f;

public:	
	ESkillKey LastSkill = ESkillKey::None;
};
