// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneWave.generated.h"

class USphereComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AArcaneWave : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneWave();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	virtual void DestroySKill() override;

	FORCEINLINE void SetMaxRadius(float Radius) {MaxDist = Radius;};
private:
	void SetInAndOutRadius(float DeltaTime);

	UFUNCTION()
  	void OnOverlaping_OutSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetWaveSize();

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* WaveOutSphere;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* WaveEffect;

	float MaxDist = 1.0f;

	UPROPERTY(EditAnywhere)
	float WaveWidth = 100.0f;

	float OutRadius = 0.0f;

	UPROPERTY(EditAnywhere)
	float WaveSpeed = 100.0f;
};
