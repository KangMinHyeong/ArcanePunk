#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "GameElements/Volume/SpawnVolume/DataTable/APBattleStageDataTable.h"
#include "FSpawnerData.generated.h"

USTRUCT(BlueprintType)
struct FSpawnerData : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnStep> SpawnStep;

	// 필요하다면 SpawnStep 멤버도 추가
	// 예시:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TArray<FSpawnStep> SpawnStep;
}; 