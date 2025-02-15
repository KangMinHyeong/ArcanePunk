// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
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
struct FStatusData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FStatusData() {}

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	int32 CharacterIndex = 0;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	uint8 CharacterGroup = 0;//0:OC / 1:NPC

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FName CharacterName = TEXT("Player");

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

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	float ShieldHP = 0.0f;
	//
};

USTRUCT(BlueprintType)
struct FCharaterUIData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FCharaterUIData() {}

	UPROPERTY(EditAnywhere)
	FString CharaterName;

	UPROPERTY(EditAnywhere)
	TMap<uint8, UTexture2D*> Portraits;
};

USTRUCT(BlueprintType)
struct FDropData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FDropData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NPCIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NPCName = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropArcaneEnergy_Percent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DropArcaneEnergy_Min = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DropArcaneEnergy_Max = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropGold_Percent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropGold_Min = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropGold_Max = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropDice_Percent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DropDice_Min = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DropDice_Max = 1;
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

