
#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_1.generated.h"

class AArcaneRain;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_1 : public USkillNumberBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void OnSkill() override;

	virtual void Remove_Skill() override;

	void Activate_UltSkill1();
	void UltSkill1_End();

private:
	void AddAbilityList();	

	UPROPERTY()
	AArcaneRain* ArcaneRain;
};
