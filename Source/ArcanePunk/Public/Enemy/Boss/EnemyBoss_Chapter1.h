// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_BossBase.h"
#include "EnemyBoss_Chapter1.generated.h"

UCLASS()
class ARCANEPUNK_API AEnemyBoss_Chapter1 : public AEnemy_BossBase
{
	GENERATED_BODY()
public:
	AEnemyBoss_Chapter1();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void MeleeAttack_1();
	void MeleeAttack_2();

	void RangeAttack_1();
	void RangeAttack_2();
	void RangeAttack_3();

	void TraceAttack_1();
	void TraceAttack_2();


};
