#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "FStatusData.generated.h"

USTRUCT(BlueprintType)
struct FStatusData : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

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

protected:
	virtual void AddValidationList(TArray<FValidationEntry>& OutValidationList) const override
	{
		OutValidationList.Add(FValidationEntry{
			[this]() { return CharacterGroup == 1 || CharacterGroup == 0; },
			TEXT("CharacterGroup must be 0 or 1.")
		});
	}
}; 