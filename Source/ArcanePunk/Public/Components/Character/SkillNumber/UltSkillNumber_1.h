
#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_1.generated.h"

class AArcaneRain;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_1 : public USkillNumberBase
{
	GENERATED_BODY()
public:
	UUltSkillNumber_1();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill() override;

	virtual void OnSkill() override;

	virtual void Activate_Skill() override;
	virtual void SkillEnd() override;

	virtual void UpdateSkillData() override;

private:
	TWeakObjectPtr<AArcaneRain> ArcaneRain;

};
