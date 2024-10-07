// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "AreaBeam.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API AAreaBeam : public AAPSkillActorBase
{
	GENERATED_BODY()
	
public:	
	AAreaBeam();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	void SetBeamRotation(FVector TargetVector);
	FORCEINLINE void SetDestroyTime(float Time) {DestroyTime = Time;};
	FORCEINLINE void SetAreaLength(float NewValue) {AreaLength = NewValue;};
	FORCEINLINE void SetAreaWidth(float NewValue) {AreaWidth = NewValue;};

private:
	virtual void DestroySKill() override;
	void OnBeamAttack();
	void SetAreaDamage();

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* AreaBeamRoot;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* AreaStartEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* AreaBeamEffect;

	FTimerHandle FireTimerHandle;

	UPROPERTY(EditAnywhere)
	float BeamDelay = 2.0f;

	UPROPERTY(EditAnywhere)
	float AreaDamageRate = 0.5f;

	float AreaLength = 1000.0f;
	float InitLength = 750.0f; // 기준값 수정X
	float AreaWidth = 15.0f;
	float InitWidth = 50.0f; // 기준값 수정X

	FVector BeamEnd;
};
