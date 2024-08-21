// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "APGameState.generated.h"

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_BODY()

	FGameData() {}

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FString SaveSlotName = TEXT("GameSlot_0");

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FString LevelName = TEXT("Test1");

	UPROPERTY(SaveGame, BlueprintReadWrite)
	float SaveRealTime = 0.0f;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FDateTime SaveDateTime = {};
};

UCLASS()
class ARCANEPUNK_API AAPGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	void SaveGameData();

	void InitGameData();

	void UpdateGameData(FGameData& GD);

public:
	UPROPERTY(Transient, BlueprintReadWrite)
	FGameData GameData;

};
