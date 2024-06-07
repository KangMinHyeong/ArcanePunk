// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneBlast.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS()
class ARCANEPUNK_API AArcaneBlast : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneBlast();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);
	
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	FORCEINLINE void SetMaxDestroyTime(float Time) {DestroyTime = Time;};
private:
	virtual void DestroySKill() override;
	void DestroySKill_Immediate();
	
private:
	UPROPERTY(EditAnywhere)
	USphereComponent* BlastTrigger;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* BlastEffectComp;	

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* BlastHitEffect;

	UPROPERTY(EditAnywhere)
	float BlastSpeed = 1000.0f;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* BlastMoveComp;

	UPROPERTY(EditAnywhere)
	float DestroyDelayTime = 0.75f;
};
