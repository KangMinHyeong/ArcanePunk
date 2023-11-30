// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "APSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UAPSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	void InitSaveData();

public:
	//PlayerData
	UPROPERTY()
	bool bSave = false;

	UPROPERTY()
	float SaveHP = 100.0f;

	UPROPERTY()
	int32 SaveMP = 100;

	UPROPERTY()
	float SaveATK = 25.0f;

	UPROPERTY()
	int32 SaveATKSpeed = 1;

	UPROPERTY()
	float SaveDEF = 0.0f;

	UPROPERTY()
	float SaveMoveSpeed = 500.0f;

	UPROPERTY()
	int32 SaveSP = 500.0f;

	UPROPERTY()
	FVector SavePlayerLocation = FVector(0,0,0);

	// GameData
	UPROPERTY()
	FName SaveLevelName = TEXT("Test1");
};
