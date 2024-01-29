// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneBeam.generated.h"

class UNiagaraSystem;
class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API AArcaneBeam : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneBeam();

	virtual void DestroySKill() override;

protected:
	virtual void BeginPlay() override;	

public:
	virtual void SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility) override;

	void SetBeamEffect();
	void SetBeamAttack();
	void SetDistance(float Dist) {Distance = Dist;};
	void SetWide(float Radius) {BeamRadius = Radius;};
protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BeamBox;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* BeamEffect;

	UPROPERTY()
	UNiagaraComponent* BeamComp;
	
	UPROPERTY(EditAnywhere)
	float Distance = 1000.0f;

	UPROPERTY(EditAnywhere)
	FVector BeamScale = FVector(1,1,1);

	UPROPERTY()
	FVector BeamStart = FVector(0,0,0);

	UPROPERTY()
	FVector BeamEnd = FVector(0,0,0);

	UPROPERTY(EditAnywhere)
	float BeamRadius = 15.0f;

	FTimerHandle BeamHitLoopTimerHandle;

	UPROPERTY(EditAnywhere)
	float BeamHitLoopTime = 0.3f;

};
