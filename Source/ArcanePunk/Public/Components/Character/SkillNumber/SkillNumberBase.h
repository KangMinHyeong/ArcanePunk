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
	UFUNCTION()
	virtual void Remove_Skill();	
	
	virtual void RemoveEffect();

	UFUNCTION()
	virtual void OnSkill();
	UFUNCTION()
	virtual void SkillEnd();
	UFUNCTION()
	virtual void Activate_Skill();
	UFUNCTION()
	virtual void Enhance();
	UFUNCTION()
	virtual void DoubleEnhance();

	virtual void MarkingOn(AActor* OtherActor, float Time);
	virtual void MarkErase();
	virtual void UpdateSkillData();
	virtual void OnCoolDown();
	virtual bool CheckSkillCondition();

	void SetMouseCursor(AArcanePunkPlayerController *PC, ESkillCursor NewCursor);
	bool CheckSmartKey(ESkillKey WhichKey);
	void SkillCancel();

	void ActivateSkillRange_Round(float Range);
	void ActivateSkillRange_Target(float Range_1, float Range_2, ESkillRangeType SkillRangeType);

	UFUNCTION()
	void CharacterRotation();
	void CharacterRotation_TwoCircle();
	void CharacterRotation_Cursor(FHitResult& Hit);
	void CharacterRotation_Sector();

	FORCEINLINE ESkillNumber GetCurrentSkillNumber() const {return CurrentSkillNumber;};
	FORCEINLINE void SetCurrentSkillNumber(ESkillNumber UpdateSkillNumber) {CurrentSkillNumber = UpdateSkillNumber;};
	
	// Cool Time
	FORCEINLINE float GetOriginCoolTime() const {return SkillNameListData.CoolTime;};
	FORCEINLINE float GetCoolTime() const {return CurrentCoolTime;};
	FORCEINLINE void SetCoolTime(float NewCoolTime) {CurrentCoolTime = NewCoolTime;};
	FORCEINLINE void AddCoolTime(float Add) {AddSkillCoolTime = Add;};

	// ChargeNum , 특정 스킬만
	FORCEINLINE void SetMaxChargeNum(uint8 NewValue) {MaxChargeNum = NewValue;};
	FORCEINLINE uint8 GetMaxChargeNum() const {return MaxChargeNum;};
	FORCEINLINE void SetCurrentChargeNum(uint8 NewValue) {CurrentChargeNum = NewValue;};
	FORCEINLINE uint8 GetCurrentChargeNum() const {return CurrentChargeNum;};

	// 추가 지속시간
	FORCEINLINE float GetAddDuration() const {return AddDuration;};
	FORCEINLINE void SetAddDuration(float NewValue) {AddDuration = AddDuration + NewValue;};

	// MP 소모값
	FORCEINLINE void SetMPConsumption(uint8 NewValue) {MPConsumption = NewValue;};

	// Skill Key
	FORCEINLINE ESkillKey GetSkillKey() const {return SkillKey;};
	FORCEINLINE void SetSkillKey(ESkillKey NewSkillKey) {SkillKey = NewSkillKey;};

	// 스킬 범위 표시 액터
	FORCEINLINE TWeakObjectPtr<AAPSkillRange> GetSkillRange_Circle() const {return SkillRange_Circle;};

	// 스킬 활성화 여부
	FORCEINLINE bool IsActivate() const {return bActivate;};
	
	// 스킬 증강 정보
	FORCEINLINE FSkillAbilityNestingData GetSkillAbilityNestingData() const {return SkillAbilityNestingData;};
	void ClearSkillAbilityNestingData(EEnHanceType ClearEnHanceType);
	void AddSkillAbilityNestingData(EEnHanceType AddEnHanceType, TPair<uint8, uint16> UpdateAbilityNestingNum);

protected:
	virtual void Spawn_SkillRange();

protected:
	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<AArcanePunkPlayerController> OwnerCharacterPC;

	ESkillTypeState CurrentSkillType = ESkillTypeState::Type_None;

	// TArray<ESkillAbility> CurrentSkillAbility;

	FHitResult HitResult;

	bool Skilling = false;

	float SpawnAddLocation = 35.0f;

	float RotSpeed = 640.0f;

	uint8 MPConsumption = 1;
	float CurrentCoolTime = 5.0f;
	float AddSkillCoolTime = 0.0f;

	uint8 CurrentChargeNum = 0;
	uint8 MaxChargeNum = 0;
	uint8 MaxChargeNum_Origin = 0;

	float AddDuration = 0.0f;

	FVector SpawnLocation;
	FVector TargetLocation;
	FRotator SpawnRotation;

	FSkillAbilityRowNameData* RowDataTable;
	FSkillAbilityDataSheet* AbilityData;

	UPROPERTY() // 스킬 기본값
	FSkillNameList SkillNameListData;
	UPROPERTY() // 스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData SkillAbilityNestingData;

	ESkillNumber CurrentSkillNumber = ESkillNumber::None;
	ESkillKey SkillKey = ESkillKey::None;

	bool bActivate = false;

	TWeakObjectPtr<AAPSkillRange> SkillRange_Circle;
	TWeakObjectPtr<AAPSkillRange> SkillRange_Target;
	TWeakObjectPtr<AAPSkillRange_TwoCircle> SkillRange_TWoCircle;

};
