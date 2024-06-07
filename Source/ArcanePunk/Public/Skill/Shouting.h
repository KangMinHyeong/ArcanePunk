// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "Shouting.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API AShouting : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AShouting();

	virtual void DestroySKill() override;

protected:
	virtual void BeginPlay() override;	

	virtual void CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum) override;

public:
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	
	void SetShoutingEffect();
	void SetShoutingAttack();

private:
	UPROPERTY()
	UBoxComponent* ShoutingRoot;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ShoutEffect;

	TWeakObjectPtr<UNiagaraComponent> ShoutComp;

	UPROPERTY(EditAnywhere)
	FVector ShoutScale = FVector(1,1,1);

	UPROPERTY(EditAnywhere)
	float ShoutRadius = 550.0f;

	float InitShoutRadius;
};	
