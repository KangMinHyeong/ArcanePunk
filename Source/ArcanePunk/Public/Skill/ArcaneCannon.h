// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneCannon.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS()
class ARCANEPUNK_API AArcaneCannon : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneCannon();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);
	
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	FORCEINLINE void SetMaxCannonDist(float Dist) {MaxCannonDist = Dist;};
	FORCEINLINE void SetCannonWidth(float Width) {CannonWidth = Width;};

private:
	virtual void DestroySKill() override;
	void DestroySKill_Immediate();
	
private:
	UPROPERTY(EditAnywhere)
	USphereComponent* CannonTrigger;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* CannonEffect;	

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* CannonHitEffect;

	UPROPERTY(EditAnywhere)
	float CannonSpeed = 1350.0f;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* CannonMoveComp;

	float MaxCannonDist = 0.0f;
	float CannonWidth;
	float DefaultCannonWidth = 30.0f;

	UPROPERTY(EditAnywhere)
	float ExplosionRange = 2.0f;
};
