// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/APHUD.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Components/ActorComponent.h"
#include "SkillNumberBase.generated.h"



class AArcanePunkPlayerController;
class AAPSkillRange;
class AAPSkillRange_Target;
class AArcanePunkCharacter;
class AArcanePunkPlayerController;
class AAPSkillRange_TwoCircle;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API USkillNumberBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillNumberBase();

protected:
	virtual void BeginPlay() override;
	void InitSkillData();

	void CheckingOtherSkill();
	void CursorImmediately();
	bool CheckSkillCool(ESkillKey WhichKey);
	
	virtual void AddAbilityList();

	void UpdatAbilityData(EEnHanceType EnHanceType, uint8 AbilityNum);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CreateInit();

	virtual void PlaySkill();
	virtual void OnSkill();
	virtual void Remove_Skill();	
	virtual void SkillEnd();
	virtual void RemoveEffect();
	virtual void Activate_Skill();
	virtual void Enhance();
	virtual void DoubleEnhance();
	virtual void MarkingOn(AActor* OtherActor, float Time);
	virtual void MarkErase();
	virtual void UpdateSkillData();
	virtual void OnCoolDown();

	void SetMouseCursor(AArcanePunkPlayerController *PC, ESkillCursor NewCursor);
	bool CheckSmartKey(ESkillKey WhichKey);
	void SkillCancel();

	void ActivateSkillRange_Round(float Range);
	void ActivateSkillRange_Target(float Range_1, float Range_2, ESkillRangeType SkillRangeType);

	void CharacterRotation();
	void CharacterRotation_TwoCircle();
	void CharacterRotation_Cursor(FHitResult& Hit);
	void CharacterRotation_Sector();
	
	// Cool Time
	FORCEINLINE float GetOriginCoolTime() const {return OriginCoolTime;};
	FORCEINLINE float GetCoolTime() const {return CurrentCoolTime;};
	FORCEINLINE void SetCoolTime(float NewCoolTime) {CurrentCoolTime = NewCoolTime;};

	// ChargeNum , 특정 스킬만
	FORCEINLINE void SetMaxChargeNum(uint8 NewValue) {MaxChargeNum = NewValue;};
	FORCEINLINE uint8 GetMaxChargeNum() const {return MaxChargeNum;};
	FORCEINLINE void SetCurrentChargeNum(uint8 NewValue) {CurrentChargeNum = NewValue;};
	FORCEINLINE uint8 GetCurrentChargeNum() const {return CurrentChargeNum;};

	// 추가 지속시간
	FORCEINLINE float GetAddDuration() const {return AddDuration;};
	FORCEINLINE void SetAddDuration(float NewValue) {AddDuration = AddDuration + NewValue;};
	
protected:
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<AArcanePunkPlayerController> OwnerCharacterPC;

	ESkillTypeState CurrentSkillType = ESkillTypeState::Type_None;

	// TArray<ESkillAbility> CurrentSkillAbility;

	FHitResult HitResult;

	bool Skilling = false;

	float SpawnAddLocation = 35.0f;

	uint8 MPConsumption = 1;

	float RotSpeed = 640.0f;

	float OriginCoolTime = 5.0f;
	float CurrentCoolTime = 5.0f;

	uint8 CurrentChargeNum = 0;
	uint8 MaxChargeNum = 0;
	uint8 MaxChargeNum_Origin = 0;

	float AddDuration = 0.0f;

	FVector SpawnLocation;
	FVector TargetLocation;
	FRotator SpawnRoatation;

	FSkillAbilityRowNameData* RowDataTable;
	FSkillAbilityDataSheet* AbilityData;

	
public:
	// TArray<ESkillAbility> EnableSkillAbilityList;

	UPROPERTY() // 스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData SkillAbilityNestingData;

	ESkillKey SkillKey = ESkillKey::None;

	TWeakObjectPtr<AAPSkillRange> SkillRange_Circle;

	TWeakObjectPtr<AAPSkillRange> SkillRange_Target;

	TWeakObjectPtr<AAPSkillRange_TwoCircle> SkillRange_TWoCircle;

	bool bActivate = false;

	

};
