// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/AIController/EnemyBaseAIController.h"
#include "EnemyBossBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AEnemyBossBaseAIController : public AEnemyBaseAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

};
