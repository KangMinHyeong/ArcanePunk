// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterfaceExport.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneRain.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API AArcaneRain : public AAPSkillActorBase, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()
public:	
	AArcaneRain();

protected:
	virtual void BeginPlay() override;	

public:
	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void SetRainAttack(const FVector & CollisionLocation);

private:
	virtual void DestroySKill() override;

	virtual void CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum) override;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* RainRoot;

	// UPROPERTY(EditAnywhere , BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	// UNiagaraSystem* RainEffect;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UNiagaraComponent* RainComp;

	UPROPERTY(EditAnywhere)
	FVector RainScale = FVector(1,1,1);

	UPROPERTY(EditAnywhere)
	float RainRadius = 550.0f;

	UPROPERTY(EditAnywhere)
	float RainSize = 1.0f;
	float RainSpawnRate = 40.0f;
};
