// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "SuperiorMode.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class ASuperiorAttack;

UCLASS()
class ARCANEPUNK_API ASuperiorMode : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	ASuperiorMode();

protected:
	virtual void BeginPlay() override;	
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	
	virtual void DestroySKill() override;
	
private:
	void SpawnAddtionalAttack(float Dist, float Width);

private:
	UPROPERTY()
	USceneComponent* SuperiorModeRoot;		

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SuperiorModeEffect;

	UPROPERTY(EditAnywhere)
	float MaxRange = 2.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASuperiorAttack> SuperiorAttackClass;
};
