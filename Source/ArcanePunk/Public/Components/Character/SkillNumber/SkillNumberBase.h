// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None);	
	virtual void OnSkill();
	virtual void Remove_Skill();	

	void SetMouseCursor(AArcanePunkPlayerController *PC, ESkillCursor NewCursor);
	bool CheckSmartKey(ESkillKey WhichKey, class AArcanePunkCharacter* OwnerCharacter);
	void SkillCancel();

	void ActivateSkillRange_Round(float Range);
	void ActivateSkillRange_Target(float Range_1, float Range_2, ESkillRangeType SkillRangeType);

	void CharacterRotation();

protected:
	UPROPERTY()
	ESkillTypeState CurrentSkillType = ESkillTypeState::Type_None;

	UPROPERTY()
	TArray<ESkillAbility> CurrentSkillAbility;

	UPROPERTY()
	FHitResult HitResult;

	UPROPERTY()
	bool Skilling = false;

	UPROPERTY()
	float SpawnAddLocation = 35.0f;

public:
	UPROPERTY()
	TArray<ESkillAbility> EnableSkillAbilityList;

	UPROPERTY()
	ESkillKey SkillKey = ESkillKey::None;

	UPROPERTY()
	AAPSkillRange* SkillRange_Circle;

	UPROPERTY()
	AAPSkillRange* SkillRange_Target;

	UPROPERTY()
	bool bActivate = false;

};
