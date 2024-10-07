// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneRage.generated.h"

class UBoxComponent;
class UParticleSystemComponent;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API AArcaneRage : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneRage();

protected:
	virtual void BeginPlay() override;	
	
	void ApplyRecovery();

public:
	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	virtual void CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	
	virtual void DestroySKill() override;
	
	void SetRageMode();
	
private:
	UPROPERTY()
	UBoxComponent* RageRoot;	

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* RageEffect;	

	bool bPluslusInstantDeath = false;
	float PlusInstantDeathPercent = 0.0f;

	uint8 ReflectingModeGauge = 0;

	bool bHPRecovery = false;
	float HPRecoveryPercent = 0.0f;
};
