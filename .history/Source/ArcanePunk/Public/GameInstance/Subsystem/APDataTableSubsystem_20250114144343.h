// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
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

	// DataTable
	FORCEINLINE UDataTable* GetSkillNameListDataTable() const {return SkillNameListDataTable;};
	FORCEINLINE UDataTable* GetSkillAbilityRowDataTable() const {return SkillAbilityRowDataTable;};
	FORCEINLINE UDataTable* GetSilverAbilityDataTable() const {return SilverAbilityDataTable;};
	FORCEINLINE UDataTable* GetGoldAbilityDataTable() const {return GoldAbilityDataTable;};
	FORCEINLINE UDataTable* GetPlatinumAbilityDataTable() const {return PlatinumAbilityDataTable;};
	FORCEINLINE UDataTable* GetEquipDataTable() const {return EquipDataTable;};
	FORCEINLINE UDataTable* GetStringDataTable() const {return StringDataTable;};
	FORCEINLINE UDataTable* GetSequenceStringDataTable() const {return SequenceStringDataTable;};
	FORCEINLINE UDataTable* GetStatusDataTable() const {return CharacterDataTable;};
	FORCEINLINE UDataTable* GetDropDataTable() const {return DropDataTable;};

	// BP_Class
	FORCEINLINE TSubclassOf<AAPManaEnergy> GetManaEnergyClass() const {return ManaEnergyClass;};
	FORCEINLINE TSubclassOf<AEnemy_DropBase> GetDropGoldClass() const {return DropGoldClass;};
	FORCEINLINE TSubclassOf<UUserWidget> GetCheckUIClass() const {return CheckUIClass;};
	FORCEINLINE TSubclassOf<UUserWidget> GetSystemMessageClass() const {return SystemMessageClass;};
	
	FORCEINLINE TObjectPtr<UEnum> GetCheckEnum() const {return CheckEnum;};

	static void DisplaySystemMesseage(UAPDataTableSubsystem* DataTableGI, EStringRowName StringRowName,  bool bLowMessage, bool bWarning);
	static void SetTextBlock(UAPDataTableSubsystem* DataTableGI, UTextBlock *TextBlock, const EStringRowName & RowName);
	static void SetTextBlock_Name(UAPDataTableSubsystem* DataTableGI, UTextBlock *TextBlock, const FName & RowName);
	const FString & GetStringContent(const EStringRowName & RowName);

	

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
	UDataTable* SequenceStringDataTable;
	UPROPERTY()
	UDataTable* CharacterDataTable;
	UPROPERTY()
	UDataTable* DropDataTable;

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

	// UPROPERTY()
	TMap<int32, TArray<FDialogueDataTable*>> GroupedDialogueRows;
};
