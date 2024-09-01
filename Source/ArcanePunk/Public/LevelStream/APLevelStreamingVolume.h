// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelStreamingVolume.h"
#include "APLevelStreamingVolume.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AAPLevelStreamingVolume : public ALevelStreamingVolume
{
	GENERATED_BODY()

public:
	virtual void PostLoad() override;	

	UFUNCTION()
	void OnLevelLoaded();
	void CheckAllActorsLoaded();
	void HideLoadingScreen();

private:
	TWeakObjectPtr<ULevelStreaming> StreamingLevel;
	FTimerHandle CheckActorsHandle;
};
