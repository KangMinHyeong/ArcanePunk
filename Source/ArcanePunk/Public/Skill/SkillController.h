// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Skill/SkillActors/SkillActor.h"
#include "CoreMinimal.h"
#include "SkillController.generated.h"

class AAPSkillRange;
class AArcanePunkCharacter;
class ASkillActor;

UCLASS()
class ARCANEPUNK_API ASkillController : public APlayerController
{
	GENERATED_BODY()

public:
	ASkillController();

public:
	// Getter 함수도 안전하게 수정
	FORCEINLINE ASkillActor* GetSkillActor() const { return SkillActor; }
	FORCEINLINE AAPSkillRange* GetSkillTargetRange() {return SkillRange_Target.Get();};
	FORCEINLINE bool IsReady() const {return bReady;};
	
	virtual void UseSkill(ESkillKey SkillKey);

	void InitializeSkills(ESkillKey SkillKey, FName SkillId, TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter);

	void ShowSkillRange(float Range, FVector Location);

	void RotatePlayer();
	void Restore();

	void UpdateSkillSlotImage();
	
private:
	UFUNCTION()
	void ActivateSkill();

	bool CheckSkillCondition(ESkillKey SkillKey);
	bool CheckSkillCool(ESkillKey SkillKey);

	void StartCoolDown();
private:
	UPROPERTY()
	FSkillControllerData CurSkillData;
	UPROPERTY()
	ASkillActor* SkillActor;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
	TWeakObjectPtr<UAnimMontage> SkillAction;
	
	TWeakObjectPtr<AAPSkillRange> SkillRange_Target;
	TWeakObjectPtr<AAPSkillRange> SkillRange_Ground;

	bool bReady = false;

	FVector TargetLocation;

	ESkillKey CurrentSkillKey;
};

