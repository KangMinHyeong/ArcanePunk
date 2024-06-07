// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "WindCutter.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AWindCutter : public AAPSkillActorBase
{
	GENERATED_BODY()
	
public:	
	AWindCutter();

protected:
	virtual void BeginPlay() override;
	virtual void DestroySKill() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	FORCEINLINE void SetMaxWindWitdh(float Width) {MaxWindWitdh =  Width;};
	void SetMaxLocation(FVector Location);
	FORCEINLINE void SetMaxRadius(float Radius) {MaxRadius =  Radius;};
private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	void SetWindTrigger(float DeltaTime);
	void SetWindSpeed(float DeltaTime);

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* WindRoot;

	UPROPERTY(EditAnywhere)
	USphereComponent* WindTrigger;
	
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* WindEffect;

	// UPROPERTY(EditAnywhere)
	// UNiagaraSystem* WindEffect;
	// UPROPERTY(EditAnywhere)
	// UNiagaraSystem* WindGroundEffect;

	UPROPERTY(EditAnywhere)
	float WindSpeed = 2.0f;	
	float WitdhSpeed;
	float LocationSpeed;
	float RadiusSpeed;

	float EffectScale = 0.3f;

	float CurrentWindWitdh;
	float MaxWindWitdh;

	FVector CurrentLocation;
	FVector MaxLocation;

	float CurrentRadius;
	float MaxRadius;

	// UPROPERTY()
	// TArray<AActor*> HitActors;
};
