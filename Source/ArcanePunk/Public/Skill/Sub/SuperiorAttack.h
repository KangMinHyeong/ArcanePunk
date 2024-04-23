// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "SuperiorAttack.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API ASuperiorAttack : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	ASuperiorAttack();

protected:
	virtual void BeginPlay() override;	
	
public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void DestroySKill() override;
	
	FORCEINLINE void SetMoveSpeed(float Speed) {MoveSpeed = Speed;};
	void SetTargetAndWidth(FVector Target, float Width);

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
		
private:
	UPROPERTY(EditAnywhere)
	USphereComponent* SuperiorAttackTrigger;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* SuperiorAttackEffect;

	FVector CurrentLocation;
	FVector TargetLocation;

	FVector InitScale;

	float InitWidth;
	float CurrentWidth;
	float TargetWidth;	

	UPROPERTY(EditAnywhere)
	float InitSpeed;	

	float MoveSpeed;
	float ScaleSpeed;

	bool bActivate = false;
	
	UPROPERTY()
	TArray<AActor*> Actors;
};
