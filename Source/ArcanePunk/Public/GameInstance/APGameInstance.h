// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Engine/GameInstance.h"
#include "APGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillEnhanceDataUpdate, ESkillKey, FSkillAbilityNestingData);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillEnhanceDataClear, ESkillKey);

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

public:
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

	UPROPERTY()
	FGameSoundVolume GameSoundVolume;

	UPROPERTY() // Q스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData QSkillAbilityNestingData;
	UPROPERTY() // E스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData ESkillAbilityNestingData;
	UPROPERTY() // R스킬 증강 정보 (종류 및 중첩 상태)
	FSkillAbilityNestingData RSkillAbilityNestingData;

	FOnSkillEnhanceDataUpdate OnSkillEnhanceDataUpdate;
	FOnSkillEnhanceDataClear OnSkillEnhanceDataClear;
};
