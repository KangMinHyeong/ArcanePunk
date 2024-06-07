
#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber9.generated.h"

class AArcaneMine;

UCLASS()
class ARCANEPUNK_API USkillNumber9 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	USkillNumber9();
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill() override;
	
	virtual void OnSkill() override;

	virtual void Activate_Skill() override;

	virtual void SkillEnd() override;	

	virtual void UpdateSkillData() override;

	virtual bool CheckSkillCondition() override;

private:
	virtual void AddAbilityList() override;
	virtual void Spawn_SkillRange() override;	

private:
	TWeakObjectPtr<AArcaneMine> ArcaneMine;

	float Skill9_TargetRange = 120.0f;
	float Skill9_TargetRange_Origin = 120.0f;

	float Skill9_LimitDistance = 350.0f;
	float Skill9_LimitDistance_Origin = 350.0f;
};
