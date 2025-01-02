// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillActor.h"
#include "CoreMinimal.h"
#include "SkillController.generated.h"

class AAPSkillRange;

UCLASS()
class ARCANEPUNK_API ASkillController : public APlayerController
{
	GENERATED_BODY()

public:
	ASkillController();

	FORCEINLINE ASkillActor* GetSkillActor() {return SkillActor;};
	
	virtual void UseSkill(ESkillKey SkillKey);

	void InitializeSkills(FName skillName, AArcanePunkCharacter* OwnerCharacter);

	void ShowSkillRange(float Range, FVector Location);

private:
	UFUNCTION()
	void ActivateSkill();

	void Restore();

	bool CheckSkillCondition(ESkillKey SkillKey);
	bool CheckSkillCool(ESkillKey SkillKey);


private:
	UPROPERTY()
	ASkillActor* SkillActor;

	TWeakObjectPtr<AArcanePunkCharacter> OwnerPlayer;

	TWeakObjectPtr<AAPSkillRange> SkillRange_Target;

	bool bReady = false;

	FVector TargetLocation;

	ESkillKey CurrentSkillKey;
};

