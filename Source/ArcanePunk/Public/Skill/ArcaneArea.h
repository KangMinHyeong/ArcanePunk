// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneArea.generated.h"

class AAreaBeam;

UCLASS()
class ARCANEPUNK_API AArcaneArea : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneArea();

protected:
	virtual void BeginPlay() override;	

public:
	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	
private:
	virtual void DestroySKill() override;

	void SpwanAreaBeam();

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* AreaRoot;	

	UPROPERTY(EditAnywhere)
	uint8 AreaBeamNumber = 5;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAreaBeam> AreaBeamClass;

	UPROPERTY(EditAnywhere)
	float AreaDist = 700.0f;

	UPROPERTY(EditAnywhere)
	float AreaWidth = 50.0f;
};
