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

public:
	void StartTutorial(APlayerController* PC);

private:
	TutorialUIClass;

	UPROPERTY(EditAnywhere)
	float NextDelay = 2.0f;
};