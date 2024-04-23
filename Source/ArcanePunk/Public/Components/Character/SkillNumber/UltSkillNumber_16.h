
#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_16.generated.h"

class ASuperiorMode;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_16 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_16();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PlaySkill() override;

	virtual void OnSkill() override;
	
	virtual void Activate_Skill() override;

	virtual void SkillEnd() override;

	virtual void UpdateSkillData() override;
private:
	TWeakObjectPtr<ASuperiorMode> SuperiorMode;	
};
