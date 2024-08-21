// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Enemy_DroneSoldier.generated.h"

class AEnemy_Drone;

UCLASS()
class ARCANEPUNK_API AEnemy_DroneSoldier : public AEnemy_CharacterBase
{
	GENERATED_BODY()
public:	
	virtual void NormalAttack() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy_Drone> DroneClass;

	UPROPERTY(EditAnywhere)
	float DroneHeight = 200.0f;
};
