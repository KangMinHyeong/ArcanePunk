// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/Subsystem/APSoundSubsystem.h"
#include "GameInstance/Subsystem/APSettingSubsystem.h"
#include "GameInstance/Subsystem/APDataTableSubsystem.h"
#include "Engine/GameInstance.h"
#include "APGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartFadeIn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndedFadeOut);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillEnhanceDataUpdate, ESkillKey, FSkillAbilityNestingData);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillEnhanceDataClear, ESkillKey);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGettingGold, int32);

class UAPSaveGame;
class APawn;

UCLASS()
class ARCANEPUNK_API UAPGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	void UpdateSkillEnhanceData(ESkillKey UpdateSkillKey, FSkillAbilityNestingData UpdateSkillAbilityNestingData);
	void ClearSkillEnhanceData(ESkillKey UpdateSkillKey);
	
	FORCEINLINE FString GetDefaultSlotName() const {return DefaultSlotName;};
	FORCEINLINE void SetDefaultSlotName(FString NewName) {DefaultSlotName = NewName;};
	
	FORCEINLINE UAPSaveGame* GetAPSaveGameData() const {return APSaveGameData;};

	FORCEINLINE TMap<FName, bool> GetCanEnterStage() const {return CanEnterStage;};
	FORCEINLINE void SetCanEnterStage(FName KeyName, bool NewBool) {CanEnterStage[KeyName] = NewBool;};

	FORCEINLINE bool IsNewGame() const {return bNewGame;};
	FORCEINLINE void SetNewGame(bool NewBool) {bNewGame = NewBool;};

	FORCEINLINE FSkillAbilityNestingData GetQSkillAbilityNestingData() const {return QSkillAbilityNestingData;};
	FORCEINLINE FSkillAbilityNestingData GetESkillAbilityNestingData() const {return ESkillAbilityNestingData;};
	FORCEINLINE FSkillAbilityNestingData GetRSkillAbilityNestingData() const {return RSkillAbilityNestingData;};

	FORCEINLINE float GetGoldPlusPercent() const {return GoldPlusPercent;};
	FORCEINLINE void SetGoldPlusPercent(float NewValue) {GoldPlusPercent = NewValue;};
	int32 CheckGoldAmount();

	FORCEINLINE float GetSavedRealTime() const {return SavedRealTime;};
	FORCEINLINE void SetSavedRealTime(float NewValue) {SavedRealTime = NewValue;};
	
	void OpenLevel(const UObject * WorldContextObject, const FName LevelName);

	static UAPSoundSubsystem* GetSoundGI(UObject* WorldContextObject);
	static UAPDataTableSubsystem* GetDataTableGI(UObject* WorldContextObject);
	static UAPSettingSubsystem* GetSettingGI(UObject* WorldContextObject);

private:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APawn>> PlayerClass;
	UPROPERTY()
	TArray<APawn*> Players;
	UPROPERTY()
	uint8 PlayerIndex = 0;

	UPROPERTY()
	FString DefaultSlotName = "PlayerSlot_0";

	UPROPERTY()
	bool bNewGame = false;

	UPROPERTY()
	bool HasGameData = false;

	UPROPERTY(EditAnywhere)
	TMap<FName, bool> CanEnterStage; // Stage이름, 입장가능여부

	UPROPERTY() // Q스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData QSkillAbilityNestingData;
	UPROPERTY() // E스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData ESkillAbilityNestingData;
	UPROPERTY() // R스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData RSkillAbilityNestingData;

	UPROPERTY()
	float GoldPlusPercent = 0.0f;

	UPROPERTY()
	UAPSaveGame* APSaveGameData;

	UPROPERTY()
	float SavedRealTime = 0.0f;


public:
	FOnSkillEnhanceDataUpdate OnSkillEnhanceDataUpdate;
	FOnSkillEnhanceDataClear OnSkillEnhanceDataClear;
	FOnGettingGold OnGettingGold;
	
	FOnStartFadeIn OnStartFadeIn;
	FOnEndedFadeOut OnEndedFadeOut;
};
