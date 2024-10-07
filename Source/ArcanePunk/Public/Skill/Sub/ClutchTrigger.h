// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ClutchTrigger.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UAPHitPointComponent;

UCLASS()
class ARCANEPUNK_API AClutchTrigger : public AAPSkillActorBase
{
	GENERATED_BODY()
	
public:	
	AClutchTrigger();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	UFUNCTION()
  	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE USphereComponent* GetClutchSphere() {return ClutchSphere;}; 
	void SetClutchSphere(float NewValue);
	void SetSlowPercent(uint8 UpdateSlowPercent);
	void SetDestroy(float Time);

private:
	void OnDestroy();
	void CheckEnemyDeath(AActor* OtherActor);
	void DrainCheck(AActor* OtherActor, float DamageApplied);

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* ClutchSphere;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ClutchEffect;
};
