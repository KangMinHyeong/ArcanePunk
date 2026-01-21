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

	/** Get loading progress (0.0 ~ 1.0) */
	UFUNCTION(BlueprintPure, Category = "Streaming")
	float GetLoadingProgress() const;

	/** Log memory usage for profiling */
	void LogMemoryUsage() const;

private:
	UPROPERTY()
	TArray<TWeakObjectPtr<ULevelStreaming>> StreamingLevels;

	FTimerHandle CheckActorsHandle;

	/** Memory before loading */
	UPROPERTY()
	float MemoryBeforeLoad = 0.0f;

	/** Total actors to check */
	UPROPERTY()
	int32 TotalActorsToCheck = 0;

	/** Actors completed */
	UPROPERTY()
	int32 ActorsCompleted = 0; 

};
