// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Engine/GameInstance.h"
#include "APGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillEnhanceDataUpdate, ESkillKey, FSkillAbilityNestingData);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillEnhanceDataClear, ESkillKey);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGettingGold, int32);

class AEnemy_DropPackage;

USTRUCT(BlueprintType)
struct FGameSoundVolume
{
	GENERATED_USTRUCT_BODY()

	FGameSoundVolume() {}

	UPROPERTY()
	float MasterVolume = 1.0f;
	UPROPERTY()
	float BGMVolume = 1.0f;
	UPROPERTY()
	float EffectVolume = 1.0f;
};

UCLASS()
class ARCANEPUNK_API UAPGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	void UpdateSkillEnhanceData(ESkillKey UpdateSkillKey, FSkillAbilityNestingData UpdateSkillAbilityNestingData);
	void ClearSkillEnhanceData(ESkillKey UpdateSkillKey);

	// DataTable
	FORCEINLINE UDataTable* GetSkillNameList() const {return SkillNameList;};
	FORCEINLINE UDataTable* GetSkillAbilityRowData() const {return SkillAbilityRowData;};
	FORCEINLINE UDataTable* GetSilverAbilityDataTable() const {return SilverAbilityDataTable;};
	FORCEINLINE UDataTable* GetGoldAbilityDataTable() const {return GoldAbilityDataTable;};
	FORCEINLINE UDataTable* GetPlatinumAbilityDataTable() const {return PlatinumAbilityDataTable;};
	FORCEINLINE UDataTable* GetEquipDataTable() const {return EquipDataTable;};

	FORCEINLINE FString GetDefaultGameSlot() const {return DefaultGameSlot;};
	FORCEINLINE void SetDefaultGameSlot(FString NewName) {DefaultGameSlot = NewName;};
	FORCEINLINE FString GetDefaultPlayerSlot() const {return DefaultPlayerSlot;};
	FORCEINLINE void SetDefaultPlayerSlot(FString NewName) {DefaultPlayerSlot = NewName;};

	FORCEINLINE FGameSoundVolume GetGameSoundVolume() const {return GameSoundVolume;};
	// 후에 값이 변할때마다 업데이트
	FORCEINLINE void SetGameMasterVolume(float NewValue) {GameSoundVolume.MasterVolume = NewValue;};
	FORCEINLINE void SetGameBGMVolume(float NewValue) {GameSoundVolume.BGMVolume = NewValue;};
	FORCEINLINE void SetGameEffectVolume(float NewValue) {GameSoundVolume.EffectVolume = NewValue;};

	FORCEINLINE TMap<FName, bool> GetCanEnterStage() const {return CanEnterStage;};
	FORCEINLINE void SetCanEnterStage(FName KeyName, bool NewBool) {CanEnterStage[KeyName] = NewBool;};

	FORCEINLINE bool IsNewGame() const {return bNewGame;};
	FORCEINLINE void SetNewGame(bool NewBool) {bNewGame = NewBool;};

	FORCEINLINE TMap<FName , float> GetPackageDropMap() const {return PackageDropMap;};
	FORCEINLINE TSubclassOf<AEnemy_DropPackage> GetDropPackageClass() const {return DropPackageClass;};
	
	FORCEINLINE FSkillAbilityNestingData GetQSkillAbilityNestingData() const {return QSkillAbilityNestingData;};
	FORCEINLINE FSkillAbilityNestingData GetESkillAbilityNestingData() const {return ESkillAbilityNestingData;};
	FORCEINLINE FSkillAbilityNestingData GetRSkillAbilityNestingData() const {return RSkillAbilityNestingData;};

	FORCEINLINE float GetGoldPlusPercent() const {return GoldPlusPercent;};
	FORCEINLINE void SetGoldPlusPercent(float NewValue) {GoldPlusPercent = NewValue;};
	int32 CheckGoldAmount();
private:
	UPROPERTY()
	FString DefaultGameSlot = "GameSlot_0";

	UPROPERTY()
	FString DefaultPlayerSlot = "PlayerSlot_0";	

	UPROPERTY()
	bool bNewGame = false;

	UPROPERTY()
	bool HasGameData = false;

	UPROPERTY(EditAnywhere)
	TMap<FName, bool> CanEnterStage; // Stage이름, 입장가능여부

	UPROPERTY(EditAnywhere, Category = "Drop")
	TMap<FName , float> PackageDropMap; // Item ID , Drop 확률 (보따리 드랍)
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy_DropPackage> DropPackageClass;

	UPROPERTY()
	FGameSoundVolume GameSoundVolume;

	UPROPERTY() // Q스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData QSkillAbilityNestingData;
	UPROPERTY() // E스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData ESkillAbilityNestingData;
	UPROPERTY() // R스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData RSkillAbilityNestingData;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillNameList;
	UPROPERTY(EditAnywhere)
	UDataTable* SkillAbilityRowData;
	UPROPERTY(EditAnywhere)
	UDataTable* SilverAbilityDataTable;
	UPROPERTY(EditAnywhere)
	UDataTable* GoldAbilityDataTable;
	UPROPERTY(EditAnywhere)
	UDataTable* PlatinumAbilityDataTable;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	UDataTable* EquipDataTable;

	UPROPERTY()
	float GoldPlusPercent = 0.0f;

public:
	FOnSkillEnhanceDataUpdate OnSkillEnhanceDataUpdate;
	FOnSkillEnhanceDataClear OnSkillEnhanceDataClear;
	FOnGettingGold OnGettingGold;
};
