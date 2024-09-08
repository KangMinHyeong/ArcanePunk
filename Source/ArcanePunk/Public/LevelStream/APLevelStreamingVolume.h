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

public:
	virtual void BeginPlay() override;	

	UFUNCTION()
	void OnLevelLoadComplete();

	void HideLoadingScreen();

private:
	UPROPERTY()
	TArray<TWeakObjectPtr<ULevelStreaming>> StreamingLevels;

	FTimerHandle CheckActorsHandle; 

};
