// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "SpinSlash.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API ASpinSlash : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	ASpinSlash();

protected:
	virtual void BeginPlay() override;	

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData) override;

	virtual void DestroySKill() override;	

	void SetSlashWidth(float Width);
private:
	void OnKeyDownCheck(float DeltaTime);
	void OnSlashDamage();
 	void SetSlashSize(); 

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* SlashTrigger;

	UPROPERTY(EditAnywhere)	
	UNiagaraSystem* SlashEffect;

	TWeakObjectPtr<UNiagaraComponent> SlashEffectComp;

	TWeakObjectPtr<USkillNumberBase> Skill_18;

	float CurrentTime = 0.0f;

	UPROPERTY(EditAnywhere)
	float SlashAttackRate = 0.25f;

	float SlashWidth = 0.0f;

	float ChargeSpeed = 1.0f;

	float SlashSize = 1.0f;

	FTimerHandle SlashTimerHandle;

};
