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

	UPROPERTY(Transient, BlueprintReadWrite)
	int32 MyCharacterIndex = 0;

	UPROPERTY(Transient, BlueprintReadWrite)
	FName CharacterName = TEXT("Player");

	UPROPERTY(Transient, BlueprintReadWrite)
	uint8 CharacterGroup = 0;//0:OC / 1:NPC

	UPROPERTY(Transient, BlueprintReadWrite)
	uint8 Gender = 0;//0: Male / 1:Female;

	UPROPERTY(Transient, BlueprintReadWrite)
	uint8 CharacterRace = 0; //0:Human

	UPROPERTY(Transient, BlueprintReadWrite)
	uint8 CharacterType = 0; //0:TypeA 

};

// 후에 쓸 수도?
USTRUCT(BlueprintType)
struct FPlayerSkillLimitList
{
	GENERATED_USTRUCT_BODY()

	FPlayerSkillLimitList() {}

	UPROPERTY(Transient, BlueprintReadWrite)
	bool Limit_Q = false;
	UPROPERTY(Transient, BlueprintReadWrite)
	bool Limit_E = false;
	UPROPERTY(Transient, BlueprintReadWrite)
	bool Limit_R = false;
};

USTRUCT(BlueprintType)
struct FPlayerSkillLimit
{
	GENERATED_USTRUCT_BODY()

	FPlayerSkillLimit() {}

	UPROPERTY(Transient, BlueprintReadWrite)
	TMap<uint8, FPlayerSkillLimitList> SkillLimitMap;
};
//

USTRUCT(BlueprintType)
struct FPlayerDynamicData
{
	GENERATED_USTRUCT_BODY()

	FPlayerDynamicData() {}

	UPROPERTY(Transient, BlueprintReadWrite)
	bool SaveOperation = false;

	// Player Status
	UPROPERTY(Transient, BlueprintReadWrite)
	float HP = 100.0f;

	UPROPERTY(Transient, BlueprintReadWrite)
	float MaxHP = 100.0f;

	UPROPERTY(Transient, BlueprintReadWrite)
	int32 MP = 100;

	UPROPERTY(Transient, BlueprintReadWrite)
	int32 MaxMP = 100;

	UPROPERTY(Transient, BlueprintReadWrite)
	float ATK = 25.0f;

	UPROPERTY(Transient, BlueprintReadWrite)
	int32 ATKSpeed = 1;

	UPROPERTY(Transient, BlueprintReadWrite)
	float DEF = 0.0f;

	UPROPERTY(Transient, BlueprintReadWrite)
	float MoveSpeed = 500.0f;

	UPROPERTY(Transient, BlueprintReadWrite)
	int32 SP = 500.0f;

	// Player Location
	UPROPERTY(Transient, BlueprintReadWrite)
	FVector PlayerLocation = FVector(0,0,0);

	// Skill Limit
	UPROPERTY(Transient, BlueprintReadWrite)
	bool HasQSkillType = false; 	// DeathPenalty

	UPROPERTY(Transient, BlueprintReadWrite)
	bool HasESkillType = false; 	// DeathPenalty	

	UPROPERTY(Transient, BlueprintReadWrite)
	bool HasRSkillType = false; 	// DeathPenalty

	//
};

USTRUCT(BlueprintType)
struct FPlayerGoodsData
{
	GENERATED_USTRUCT_BODY()

	FPlayerGoodsData() {}

	// Reroll Dice
	// UPROPERTY(Transient, BlueprintReadWrite)
	// int32 RerollDice = 0; 	// DeathPenalty
};

USTRUCT(BlueprintType)
struct FPlayerTotalData
{
	GENERATED_USTRUCT_BODY()

	FPlayerTotalData() {}

	FPlayerStaticData PlayerStaticData;

	FPlayerDynamicData PlayerDynamicData;

	FPlayerGoodsData PlayerGoodsData;
};

UCLASS()
class ARCANEPUNK_API UAPPlayerData : public UDataTable
{
	GENERATED_BODY()

};

