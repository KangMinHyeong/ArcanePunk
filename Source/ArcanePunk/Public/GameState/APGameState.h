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
	FName LevelName = TEXT("Test1");
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

	UPROPERTY()
	TMap<FName, bool> SubStageClearMap; // 세이브 X , 인스턴스 던전 안의 Stage

};
