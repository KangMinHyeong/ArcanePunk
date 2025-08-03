// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "APItemDataStruct.generated.h"

UENUM()
enum class EItemQuality : uint8
{
	Common		UMETA(DisplayName = "Common"),
	Rare		UMETA(DisplayName = "Rare"),
	Unique		UMETA(DisplayName = "Unique"),
	Epic		UMETA(DisplayName = "Epic"),
	Legendary	UMETA(DispalyName = "Legendary"),
	Master		UMETA(DisplayName = "Master")
};

UENUM()
enum class EItemType : uint8
{
	Equipment	UMETA(DisplayName = "Equipment"),
	Consumption	UMETA(DisplayName = "Consumption"),
	Arcane		UMETA(DisplayName = "Arcane"),
	Material	UMETA(DisplayName = "Material"),
	Goods		UMETA(DisplayName = "Goods"),
	RandomBox	UMETA(DisplayName = "RandomBox"),
	Quest		UMETA(DisplayName = "Quest"),
	Etc			UMETA(DisplayName = "Etc")
};

//Minhyeong
UENUM()
enum class EEquipType : uint8
{
	No_Equip	UMETA(DisplayName = "No_Equip"),
	Weapon		UMETA(DisplayName = "Weapon")
};

USTRUCT(BlueprintType)
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float ArmorRate;

	UPROPERTY(EditAnywhere)
	float DamageValue;

	UPROPERTY(EditAnywhere)
	float RestoreAmount;

	UPROPERTY(EditAnywhere)
	bool IsBound;

	UPROPERTY(EditAnywhere)
	bool IsStotrage;

	UPROPERTY(EditAnywhere)
	bool IsCraft;

	UPROPERTY(EditAnywhere)
	bool IsDisassemble;

	UPROPERTY(EditAnywhere)
	bool IsQuickSlot;

	UPROPERTY(EditAnywhere)
	int32 ItemEfficacy;

	UPROPERTY(EditAnywhere)
	FString RecipeItem;

	UPROPERTY(EditAnywhere)
	FString DisassembleResult;

	// Minhyeong
	UPROPERTY(EditAnywhere)
	EEquipType EquipType;
};

USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText;

	UPROPERTY(EditAnywhere)
	FText ConditionText;
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	bool IsStackable;

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	int32 Cost;

	UPROPERTY(EditAnywhere)
	int32 Price;

	UPROPERTY(EditAnywhere)
	int32 RepairCost;
};

USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* SkelMesh;
};

UCLASS()
class ARCANEPUNK_API UAPItemDataStruct : public UDataTable
{
	GENERATED_BODY()
	
public:
	UAPItemDataStruct();
	~UAPItemDataStruct();
}; 