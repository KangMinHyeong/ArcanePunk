// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "APSkillRange_TwoCircle.generated.h"

class UDecalComponent;

UCLASS()
class ARCANEPUNK_API AAPSkillRange_TwoCircle : public AAPSkillRange_Target
{
	GENERATED_BODY()
public:
	AAPSkillRange_TwoCircle();

	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	FORCEINLINE UDecalComponent* GetDecalComponent_2() {return RangeDecal_2;};

private:
	UPROPERTY(EditAnywhere)
	UDecalComponent* RangeDecal_2;	
};
