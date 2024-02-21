
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
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void OnSkill() override;

	void Activate_Skill9();

private:
	virtual void AddAbilityList() override;
	
	UPROPERTY()
	AArcaneMine* ArcaneMine;
};
