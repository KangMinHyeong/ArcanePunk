// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneReflector.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AArcaneReflector : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneReflector();

protected:
	virtual void BeginPlay() override;	

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData) override;

	virtual void DestroySKill() override;	

	void SetDotRange(float Radius);
private:
	void OnReflectingCheck(float DeltaTime);
	void StartReflectingMode();
	void OnReflectingMode();
	void SpawnReflector();
	void DotDamage(float Multiple = 1.0f);

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* DotTrigger;	

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* DotRangeEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ReflectorEffect;

	FTimerHandle DamageHandle;
	FTimerHandle ReflectTimerHandle;

	UPROPERTY(EditAnywhere)
	float DotRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float ReflectTime = 6.0f;

	UPROPERTY(EditAnywhere)
	float MaxflectTime = 10.0f;

	UPROPERTY(EditAnywhere)
	float CurrentReflectTime = 1.0f;

	float ChargeSpeed = 1.0f;

	float DotTriggerRange = 0.0f;
	float DotTriggerRangeSize = 0.0f;

	TWeakObjectPtr<AActor> ReflectShield;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ReflectShieldClass;
};
