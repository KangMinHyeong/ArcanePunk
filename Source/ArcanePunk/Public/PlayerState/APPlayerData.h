// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "APPlayerData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStaticData
{
	GENERATED_USTRUCT_BODY()

	FPlayerStaticData() {}

	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 MyCharacterIndex = 0;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FName CharacterName = TEXT("Player");

	UPROPERTY(SaveGame, BlueprintReadWrite)
	uint8 CharacterGroup = 0;//0:OC / 1:NPC

	UPROPERTY(SaveGame, BlueprintReadWrite)
	uint8 Gender = 0;//0: Male / 1:Female;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	uint8 CharacterRace = 0; //0:Human

	UPROPERTY(SaveGame, BlueprintReadWrite)
	uint8 CharacterType = 0; //0:TypeA 

};

// 후에 쓸 수도?
USTRUCT(BlueprintType)
struct FPlayerSkillLimitList
{
	GENERATED_USTRUCT_BODY()

	FPlayerSkillLimitList() {}

	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool Limit_Q = false;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool Limit_E = false;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool Limit_R = false;
};

USTRUCT(BlueprintType)
struct FPlayerSkillLimit
{
	GENERATED_USTRUCT_BODY()

	FPlayerSkillLimit() {}

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<uint8, FPlayerSkillLimitList> SkillLimitMap;
};
//

USTRUCT(BlueprintType)
struct FPlayerDynamicData
{
	GENERATED_USTRUCT_BODY()

	FPlayerDynamicData() {}

	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool SaveOperation = false;

	// Player Status
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	float HP = 100.0f;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	int32 MP = 3;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	int32 MaxMP = 3;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	float ATK = 25.0f;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	float ATKSpeed = 1.0f;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	float DEF = 0.0f;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	float MoveSpeed = 500.0f;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	int32 SP = 500.0f;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	float CriticalPercent = 10.0f;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	float CriticalDamageCoefficient = 2.0f;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	int32 CriticalStep = 1;

	// Player Location
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FVector PlayerLocation = FVector(0,0,0);

	//
};

USTRUCT(BlueprintType)
struct FPlayerGoodsData
{
	GENERATED_USTRUCT_BODY()

	FPlayerGoodsData() {}

	// Reroll Dice
	// UPROPERTY(SaveGame, BlueprintReadWrite)
	// int32 RerollDice = 0; 	// DeathPenalty
};

USTRUCT(BlueprintType)
struct FPlayerTotalData
{
	GENERATED_USTRUCT_BODY()

	FPlayerTotalData() {}

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FPlayerStaticData PlayerStaticData = {};

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FPlayerDynamicData PlayerDynamicData = {};

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FPlayerGoodsData PlayerGoodsData = {};

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FString SaveSlotName = TEXT("PlayerSlot_0");
};

UCLASS()
class ARCANEPUNK_API UAPPlayerData : public UDataTable
{
	GENERATED_BODY()

};

