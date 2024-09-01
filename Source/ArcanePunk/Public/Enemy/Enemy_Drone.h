// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Enemy_Drone.generated.h"

class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AEnemy_Drone : public AEnemy_CharacterBase
{
	GENERATED_BODY()
public:
	AEnemy_Drone();

	virtual void NormalAttack() override;

	virtual void OnDeath_MontageEnded() override;

protected:
	virtual void BeginPlay() override;	

private:
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DestroyEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* LaserEffect;

	UPROPERTY(EditAnywhere)
	USceneComponent* DestroyPoint;
};
