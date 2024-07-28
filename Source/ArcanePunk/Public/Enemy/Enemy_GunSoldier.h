// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Enemy_GunSoldier.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AEnemy_GunSoldier : public AEnemy_CharacterBase
{
	GENERATED_BODY()

public:
	AEnemy_GunSoldier();

	virtual void NormalAttack() override;

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* AmmoSpawnComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> AmmoClass;

};
