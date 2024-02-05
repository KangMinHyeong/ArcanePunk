// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/APSkillNumber.h"
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

	void SetSkillState(ESkillKey NewValue);
	void AddSkillState(ESkillKey NewValue);
	void RemoveSkillState(ESkillKey NewValue = ESkillKey::None);
	UFUNCTION(BlueprintPure)
	ESkillKey GetSkillState();

	FORCEINLINE UAPSkillNumber* GetAPSkillNumberComponent() const {return SkillNumComp;}; // SkillNumComp 반환

private:
	void SkillDetermine(ESkillKey First, ESkillKey Second);
	void PlayBasicSkill();
	void CastSkillNumber(ESkillNumber SkillNumber, ESkillKey WhichKey);
	void CastUltSkillNumber(EUltSkillNumber UltSkillNumber, ESkillKey WhichKey);
	bool ActivationCheck(ESkillNumber SkillNumber);
	
private:
	// Q,E, shift, space 스킬 컴포넌트
	UPROPERTY()
	class UAPSkillBaseQ* QComp;
	UPROPERTY()
	class UAPSkillBaseE* EComp;
	UPROPERTY()
	class UAPSkillBaseSpace* SpaceComp;

	// 스킬 Base State
	UPROPERTY()
	ESkillKey SkillState = ESkillKey::None;

	UPROPERTY()
	TArray<ESkillKey>SkillStateArr;

	//TimerHandle
	FTimerHandle SkillCancleTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSkillNumber* SkillNumComp;

public:	
	UPROPERTY()
	ESkillKey LastSkill = ESkillKey::None;
};
