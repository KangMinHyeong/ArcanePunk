// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APBattleStageUI.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStartingNewWave)

class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPBattleStageUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateBattleUI(int32 RemainWave, float WaveTime);	

private:
	UFUNCTION()
	void StartNewWave();

	float GetWaveTime();

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_RemainWave;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_NexWaveTime;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_RemainWaveNum;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_CurrentWaveTime;


	FTimerHandle SpawnMonsterTimerHandle;

	float CurrentWaveTime = 0.0f;
	
public:
	FOnStartingNewWave OnStartingNewWave;
};
