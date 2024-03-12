// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "Enemy_DropDice.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AEnemy_DropDice : public AEnemy_DropBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void OnDropDice();
	
};
