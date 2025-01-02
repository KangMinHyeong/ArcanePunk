// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/APGameModeBattleStage.h"
#include "APGameModeTutorialStage.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AAPGameModeTutorialStage : public AAPGameModeBattleStage
{
	GENERATED_BODY()

protected:
	virtual void StartPlay() override;

	void StartTutorial();

private:
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere)
	float FirstDelay = 3.0f;

	UPROPERTY(EditAnywhere)
	float NextDelay = 2.0f;
};
