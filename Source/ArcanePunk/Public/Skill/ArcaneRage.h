// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneRage.generated.h"

class UBoxComponent;
class UParticleSystemComponent;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API AArcaneRage : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneRage();

protected:
	virtual void BeginPlay() override;	

public:
	virtual void SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility) override;	

	virtual void DestroySKill() override;
	
	void SetRageMode();
	
private:
	UPROPERTY()
	UBoxComponent* RageRoot;	

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* RageEffect;	

	UPROPERTY()
	AArcanePunkCharacter* OwnerCharacter;
};
