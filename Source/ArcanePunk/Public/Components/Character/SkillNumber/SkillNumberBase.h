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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API USkillNumberBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillNumberBase();

protected:
	virtual void BeginPlay() override;

	void CheckingOtherSkill();
	void CursorImmediately();
	void SetAbility(ESkillKey WhichKey);

	virtual void AddAbilityList();
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CreateInit();

	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None);	
	virtual void OnSkill();
	virtual void Remove_Skill();	
	virtual void SkillEnd();
	virtual void RemoveEffect();
	virtual void Activate_Skill();
	virtual void Enhance();
	virtual void MarkingOn(AActor* OtherActor, float Time);
	virtual void MarkErase();

	void SetMouseCursor(AArcanePunkPlayerController *PC, ESkillCursor NewCursor);
	bool CheckSmartKey(ESkillKey WhichKey, class AArcanePunkCharacter* OwnerCharacter);
	void SkillCancel();

	void ActivateSkillRange_Round(float Range);
	void ActivateSkillRange_Target(float Range_1, float Range_2, ESkillRangeType SkillRangeType);

	void CharacterRotation();

	// Only Ult
	FORCEINLINE float GetUltCoolTime() const {return UltCoolTime;};
protected:
	ESkillTypeState CurrentSkillType = ESkillTypeState::Type_None;

	// TArray<ESkillAbility> CurrentSkillAbility;

	FHitResult HitResult;

	bool Skilling = false;

	float SpawnAddLocation = 35.0f;

	uint8 MPConsumption = 1;

	float RotSpeed = 640.0f;

	float UltCoolTime = 15.0f;

public:
	// TArray<ESkillAbility> EnableSkillAbilityList;

	UPROPERTY() // 스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData SkillAbilityNestingData;

	ESkillKey SkillKey = ESkillKey::None;

	TWeakObjectPtr<AAPSkillRange> SkillRange_Circle;

	TWeakObjectPtr<AAPSkillRange> SkillRange_Target;

	bool bActivate = false;

};
