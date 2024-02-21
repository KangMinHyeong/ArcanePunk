// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerState/APPlayerData.h"
#include "GameFramework/SaveGame.h"
#include "APSaveGame.generated.h"

UCLASS()
class ARCANEPUNK_API UAPSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UAPSaveGame();

	void InitSaveData();

public:
	//PlayerData
	UPROPERTY(SaveGame)
	FPlayerTotalData SavePlayerTotalData = {};

	UPROPERTY(SaveGame)
	FString SavePlayerSlot = "PlayerSlot_0";

	UPROPERTY(SaveGame)
	FString SaveGameSlot = "GameSlot_0";
	// Goods
	// UPROPERTY()
	// int32 SaveRerollDice = 0;	

	// GameData
	UPROPERTY()
	FName SaveLevelName = TEXT("Test1");
};
