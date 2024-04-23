// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "TimeRewinder.generated.h"

class UNiagaraComponent;
class USphereComponent;

UCLASS()
class ARCANEPUNK_API ATimeRewinder : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	ATimeRewinder();
protected:
	virtual void BeginPlay() override;	

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData) override;

	FORCEINLINE void SetRewinderWidth(float Width) {RewinderWidth = Width;};
	FORCEINLINE void SetRewinderLocation(FVector Location) {RewinderLocation = Location;};
	void SetTimeRewinderAttack();

	virtual void DestroySKill() override;

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* RewinderSphere;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* RewinderEffect;

	UPROPERTY(EditAnywhere)
	float RewindSpeed = 350.0f;

	float RewinderWidth = 0.0f;

	bool bRewind = false;

	FVector RewinderLocation;

};
