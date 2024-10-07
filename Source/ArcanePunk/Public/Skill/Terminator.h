// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "Terminator.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API ATerminator : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	ATerminator();

protected:
	virtual void BeginPlay() override;	
	
	void ATKBuff(float DeltaTime);
    void ATKSpeedBuff(float DeltaTime);
    void FastBuff(float DeltaTime);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	
	virtual void DestroySKill() override;
	
	void SetTerminatorMode();
	void ResettTerminatorMode();

private:
	UPROPERTY()
	USceneComponent* TerminatorRoot;	

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* TerminatorEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* TerminatorStartEffect;

	UPROPERTY()
	UNiagaraComponent* TerminatorEffectComp;

	float CurrentFastCoefficient = 0.0f;
	float CurrentATKCoefficient = 0.0f;
	float CurrentATKSpeedCoefficient = 0.0f;

	UPROPERTY(EditAnywhere)
	float BuffEndSpeed = 10.0f;

	bool bBuffOn = false;

	bool bComplete = false;
};
