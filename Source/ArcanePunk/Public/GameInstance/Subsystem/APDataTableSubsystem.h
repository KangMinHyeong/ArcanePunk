// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "Character/SkillDataTable/SkillDataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataStructs/Common/FDialogueDataTable.h"
#include "APDataTableSubsystem.generated.h"

class UTextBlock;
class AEnemy_DropBase;
class AAPManaEnergy;
class UUserWidget;

UCLASS()
class ARCANEPUNK_API UAPDataTableSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:	
	UAPDataTableSubsystem();	

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	// BP_Class
	FORCEINLINE TSubclassOf<AAPManaEnergy> GetManaEnergyClass() const {return ManaEnergyClass;};
	FORCEINLINE TSubclassOf<AEnemy_DropBase> GetDropGoldClass() const {return DropGoldClass;};
	FORCEINLINE TSubclassOf<UUserWidget> GetCheckUIClass() const {return CheckUIClass;};
	FORCEINLINE TSubclassOf<UUserWidget> GetSystemMessageClass() const {return SystemMessageClass;};
	
	FORCEINLINE TObjectPtr<UEnum> GetCheckEnum() const {return CheckEnum;};

	static void DisplaySystemMesseage(UAPDataTableSubsystem* DataTableGI, EStringRowName StringRowName,  bool bLowMessage, bool bWarning);
	static void DisplaySystemMesseage(UAPDataTableSubsystem* DataTableGI, const FName & Message,  bool bLowMessage, bool bWarning);
	static void SetTextBlock(UAPDataTableSubsystem* DataTableGI, UTextBlock *TextBlock, const EStringRowName & RowName);
	static void SetTextBlock_Name(UAPDataTableSubsystem* DataTableGI, UTextBlock *TextBlock, const FName & RowName);
	const FString & GetStringContent(const EStringRowName & RowName);

	void InitDialogueData();

	const TArray<FDialogueDataTable> GetDialogues(const int32 GroupID) const;

	void CollectDataTablesByStruct();

	template<typename T>
	const UDataTable* GetDataTableByStruct() const
	{
		static_assert(TIsDerivedFrom<T, FTableRowBase>::IsDerived, "T must be derived from FTableRowBase");
		const TSoftObjectPtr<UDataTable>* Found = AllDataTablesByStruct.Find(T::StaticStruct());
		return Found ? Found->Get() : nullptr;
	}

	template<typename T>
	const T* GetRowByStruct(const FName& RowName, const FString& ContextString = TEXT("")) const
	{
		static_assert(TIsDerivedFrom<T, FTableRowBase>::IsDerived, "T must be derived from FTableRowBase");
		const UDataTable* Table = GetDataTableByStruct<T>();
		return Table ? Table->FindRow<T>(RowName, ContextString) : nullptr;
	}

private:
	UPROPERTY()
	UDataTable* SkillNameListDataTable;
	UPROPERTY()
	UDataTable* SkillAbilityRowDataTable;
	UPROPERTY()
	UDataTable* SilverAbilityDataTable;
	UPROPERTY()
	UDataTable* GoldAbilityDataTable;
	UPROPERTY()
	UDataTable* PlatinumAbilityDataTable;
	UPROPERTY()
	UDataTable* StringDataTable;
	UPROPERTY()
	UDataTable* DialogueDataTable;
	UPROPERTY()
	UDataTable* CharacterDataTable;
	UPROPERTY()
	UDataTable* DropDataTable;
	UPROPERTY()
	UDataTable* CharacterUIDataTable;

	UPROPERTY()
	UDataTable* EquipDataTable;

	// BP_Class
	UPROPERTY()
	TSubclassOf<AAPManaEnergy> ManaEnergyClass;
	UPROPERTY()
	TSubclassOf<AEnemy_DropBase> DropGoldClass;
	UPROPERTY()
	TSubclassOf<UUserWidget> CheckUIClass;
	UPROPERTY()
	TSubclassOf<UUserWidget> SystemMessageClass;

	UPROPERTY()
	TObjectPtr<UEnum> CheckEnum;

	UPROPERTY()
	TMap<int32, FDialogueGroupData> GroupedDialogueRows;

	UPROPERTY()
	TMap<const UScriptStruct*, TSoftObjectPtr<UDataTable>> AllDataTablesByStruct;
};
