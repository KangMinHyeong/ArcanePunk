// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "BomberMan.generated.h"

class USphereComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API ABomberMan : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	ABomberMan();

protected:
	virtual void BeginPlay() override;	
	
    void FastBuff();

public:
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData) override;
	virtual void DestroySKill() override;

private:
	void OnExplosion();

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* BomberTrigger;	

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* BomberEffect;

	UPROPERTY(EditAnywhere)
	float BomberSize = 2.5f;

	UPROPERTY(EditAnywhere)
	float BomberHitRate = 1.0f;

	FTimerHandle BomberTimerHandle;

};
