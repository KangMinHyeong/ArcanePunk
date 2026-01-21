// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataStructs/Player/FStatusData.h"
#include "APPlayerData.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FPlayerStaticData
{
	GENERATED_USTRUCT_BODY()

	FPlayerStaticData() {}

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
	FStatusData StatusData = {};

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FPlayerGoodsData PlayerGoodsData = {};

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FString SaveSlotName = TEXT("PlayerSlot_0");

	// Player Location
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FVector PlayerLocation = FVector(0,0,0);

	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool SaveOperation = false;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 PlayerDeathCount = 0;

};

UCLASS()
class ARCANEPUNK_API UAPPlayerData : public UDataTable
{
	GENERATED_BODY()

};

