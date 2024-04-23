// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Enemy_ScoutDog.generated.h"

class UCapsuleComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AEnemy_ScoutDog : public AEnemy_CharacterBase
{
	GENERATED_BODY()
public:
	AEnemy_ScoutDog();

	void SpawnJumpEffect();
	void LeapFoward();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
  	void OnOverlapping(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void AttackCondition(bool NewBool);

protected:
	virtual void BeginPlay() override;
	void Test();
private:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* RushTrigger;

	UPROPERTY(EditAnywhere)
	float LeapSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* JumpEffect;

	FVector CurrentLocation;
	FVector TargetLocation;
	
};
