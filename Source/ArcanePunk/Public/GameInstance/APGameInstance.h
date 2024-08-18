// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Engine/GameInstance.h"
#include "APGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillEnhanceDataUpdate, ESkillKey, FSkillAbilityNestingData);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillEnhanceDataClear, ESkillKey);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGettingGold, int32);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnChangingSoundVolume, float, float, float) // MasterVolume, BGM, Effect

class AEnemy_DropBase;
class UAPSaveGame;
class UTextBlock;
class AAPManaEnergy;

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

	// Play Sound
	void PlayClickSound();
	void PlayUIOpenSound();
	void PlayChoiceSound();
	void PlayRejectSound();

	void SetTextBlock(UTextBlock* TextBlock, EStringRowName RowName);
	void SetTextBlock_Name(UTextBlock* TextBlock, FName RowName);
	FString GetStringContent(EStringRowName RowName);

	// DataTable
	FORCEINLINE UDataTable* GetSkillNameList() const {return SkillNameList;};
	FORCEINLINE UDataTable* GetSkillAbilityRowData() const {return SkillAbilityRowData;};
	FORCEINLINE UDataTable* GetSilverAbilityDataTable() const {return SilverAbilityDataTable;};
	FORCEINLINE UDataTable* GetGoldAbilityDataTable() const {return GoldAbilityDataTable;};
	FORCEINLINE UDataTable* GetPlatinumAbilityDataTable() const {return PlatinumAbilityDataTable;};
	FORCEINLINE UDataTable* GetEquipDataTable() const {return EquipDataTable;};
	FORCEINLINE UDataTable* GetStringData() const {return StringData;};
	FORCEINLINE UDataTable* GetSequenceStringData() const {return SequenceStringData;};
	FORCEINLINE UDataTable* GetStatusData() const {return StatusData;};
	FORCEINLINE UDataTable* GetNPCData() const {return NPCData;};
	
	FORCEINLINE FString GetDefaultSlotName() const {return DefaultSlotName;};
	FORCEINLINE void SetDefaultSlotName(FString NewName) {DefaultSlotName = NewName;};
	
	FORCEINLINE UAPSaveGame* GetAPSaveGameData() const {return APSaveGameData;};

	FORCEINLINE FGameSoundVolume GetGameSoundVolume() const {return GameSoundVolume;};
	// 후에 값이 변할때마다 업데이트
	FORCEINLINE void SetGameMasterVolume(float NewValue) {GameSoundVolume.MasterVolume = NewValue;};
	FORCEINLINE void SetGameBGMVolume(float NewValue) {GameSoundVolume.BGMVolume = NewValue;};
	FORCEINLINE void SetGameEffectVolume(float NewValue) {GameSoundVolume.EffectVolume = NewValue;};
	
	FORCEINLINE TMap<FName, bool> GetCanEnterStage() const {return CanEnterStage;};
	FORCEINLINE void SetCanEnterStage(FName KeyName, bool NewBool) {CanEnterStage[KeyName] = NewBool;};

	FORCEINLINE bool IsNewGame() const {return bNewGame;};
	FORCEINLINE void SetNewGame(bool NewBool) {bNewGame = NewBool;};

	FORCEINLINE TSubclassOf<AAPManaEnergy> GetManaEnergyClass() const {return ManaEnergyClass;};
	FORCEINLINE TSubclassOf<AEnemy_DropBase> GetDropGoldClass() const {return DropGoldClass;};
	FORCEINLINE TSubclassOf<AEnemy_DropBase> GetDropDiceClass() const {return DropDiceClass;};

	FORCEINLINE FSkillAbilityNestingData GetQSkillAbilityNestingData() const {return QSkillAbilityNestingData;};
	FORCEINLINE FSkillAbilityNestingData GetESkillAbilityNestingData() const {return ESkillAbilityNestingData;};
	FORCEINLINE FSkillAbilityNestingData GetRSkillAbilityNestingData() const {return RSkillAbilityNestingData;};


	FORCEINLINE float GetGoldPlusPercent() const {return GoldPlusPercent;};
	FORCEINLINE void SetGoldPlusPercent(float NewValue) {GoldPlusPercent = NewValue;};
	int32 CheckGoldAmount();

	FORCEINLINE float GetSavedRealTime() const {return SavedRealTime;};
	FORCEINLINE void SetSavedRealTime(float NewValue) {SavedRealTime = NewValue;};

	FORCEINLINE TSubclassOf<UUserWidget> GetCheckUIClass() const {return CheckUIClass;};

	FORCEINLINE float GetSaturation() const {return Saturation;};
	FORCEINLINE void SetSaturation(float Value) {Saturation = Value;};

	FORCEINLINE float GetContrast() const {return Contrast;};
    FORCEINLINE void SetContrast(float Value) {Contrast = Value;};

	FORCEINLINE float GetGamma() const {return Gamma;};
    FORCEINLINE void SetGamma(float Value) {Gamma = Value;};

	FORCEINLINE float GetGain() const {return Gain;};
    FORCEINLINE void SetGain(float Value) {Gain = Value;};

	FORCEINLINE float GetOffset() const {return Offset;};
    FORCEINLINE void SetOffset(float Value) {Offset = Value;};

	
private:
	UPROPERTY()
	FString DefaultSlotName = "PlayerSlot_0";

	UPROPERTY()
	bool bNewGame = false;

	UPROPERTY()
	bool HasGameData = false;

	UPROPERTY(EditAnywhere)
	TMap<FName, bool> CanEnterStage; // Stage이름, 입장가능여부


	UPROPERTY(EditAnywhere)
	TSubclassOf<AAPManaEnergy> ManaEnergyClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy_DropBase> DropGoldClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy_DropBase> DropDiceClass;

	UPROPERTY()
	FGameSoundVolume GameSoundVolume;

	// UI Sound
	UPROPERTY(EditAnywhere)
	USoundBase* ClickSound;
	UPROPERTY(EditAnywhere)
	USoundBase* UIOpenSound;
	UPROPERTY(EditAnywhere)
	USoundBase* ChoiceSound;
	UPROPERTY(EditAnywhere)
	USoundBase* RejectSound;
	UPROPERTY(EditAnywhere)
	float UISoundVolume = 1.0f;

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
	UPROPERTY(EditAnywhere)
	UDataTable* StringData;
	UPROPERTY(EditAnywhere)
	UDataTable* SequenceStringData;
	UPROPERTY(EditAnywhere)
	UDataTable* StatusData;
	UPROPERTY(EditAnywhere)
	UDataTable* NPCData;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	UDataTable* EquipDataTable;

	UPROPERTY()
	float GoldPlusPercent = 0.0f;

	UPROPERTY()
	UAPSaveGame* APSaveGameData;

	UPROPERTY()
	float SavedRealTime = 0.0f;

	// Check UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CheckUIClass;

	UPROPERTY()
	float Saturation = 0.5f;
	UPROPERTY()
	float Contrast = 0.5f;
	UPROPERTY()
	float Gamma = 0.5f;
	UPROPERTY()
	float Gain = 0.5f;
	UPROPERTY()
	float Offset = 0.5f;

	TWeakObjectPtr<UEnum> CheckEnum;
public:
	FOnSkillEnhanceDataUpdate OnSkillEnhanceDataUpdate;
	FOnSkillEnhanceDataClear OnSkillEnhanceDataClear;
	FOnGettingGold OnGettingGold;
	FOnChangingSoundVolume OnChangingSoundVolume;
};
