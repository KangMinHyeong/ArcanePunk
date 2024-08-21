// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Enemy_BossBase.generated.h"

UCLASS()
class ARCANEPUNK_API AEnemy_BossBase : public AEnemy_CharacterBase
{
	GENERATED_BODY()
public:
	AEnemy_BossBase();

	FORCEINLINE EBossPhase GetPhase() const {return BossPhase;};

protected:
	virtual void BeginPlay() override;

	void InitPatternNums();

public:
	virtual void Tick(float DeltaTime) override;

	int32 GetPatternNum();

protected:
	UPROPERTY()
	EBossPhase BossPhase = EBossPhase::Phase_1;

	UPROPERTY(EditAnywhere)
	float Phase2_HPPercent = 0.6f;

	UPROPERTY(EditAnywhere)
	TMap<FString, float> Phase1_Pattern; // Pattern, Percent

	UPROPERTY(EditAnywhere)
	TMap<FString, float> Phase2_Pattern; // Pattern, Percent

	TArray<int32> PatternNums;
};
