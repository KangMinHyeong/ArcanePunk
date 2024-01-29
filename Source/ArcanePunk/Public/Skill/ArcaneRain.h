// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneRain.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API AArcaneRain : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneRain();

protected:
	virtual void BeginPlay() override;	

public:
	virtual void SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility) override;	

	void SetRainEffect();
	void SetRainAttack();

	virtual void DestroySKill() override;
	
private:
	UPROPERTY()
	UBoxComponent* RainRoot;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* RainEffect;

	UPROPERTY()
	UNiagaraComponent* RainComp;

	UPROPERTY(EditAnywhere)
	FVector RainScale = FVector(1,1,1);

	UPROPERTY(EditAnywhere)
	float RainRadius = 550.0f;

	UPROPERTY(EditAnywhere)
	uint8 HitNumbers = 3;
};
