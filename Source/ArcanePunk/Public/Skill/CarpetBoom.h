// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "CarpetBoom.generated.h"

class UNiagaraSystem;
class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API ACarpetBoom : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	ACarpetBoom();

protected:
	virtual void BeginPlay() override;	

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData) override;

	void SetCarpetBoomWidth(float Width);
	FORCEINLINE void SetCarpetBoomLength(float Length) {CollisionLength = Length;};
	FORCEINLINE void SetBoomTargetLocation(FVector Target) {BoomTargetLocation = Target;};
	
	virtual void DestroySKill() override;
	
private:	
	UPROPERTY(EditAnywhere)
	UBoxComponent* CarpetBoomTrigger;	

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* BoomEffectComp;

	UPROPERTY()
	UNiagaraComponent* LavaEffectComp;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* LavaEffect;

	FVector BoomTargetLocation;

	UPROPERTY(EditAnywhere)
	float DefaultSpawnRate = 11.0f;

	float CollisionWidth = 0.0f;

	float CollisionLength = 0.0f;

	UPROPERTY(EditAnywhere)
	float BoomSpeed = 1000.0f;

	UPROPERTY(EditAnywhere)
	float HPRecoveryPercent = 0.3f;
	
};
