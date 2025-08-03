#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "Engine/DataTable.h"
#include "FCharacterUIData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterUIData : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	FCharacterUIData() {}

	UPROPERTY(EditAnywhere)
	FString CharacterName;

	UPROPERTY(EditAnywhere)
	TMap<uint8, UTexture2D*> Portraits;
}; 