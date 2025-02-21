// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "APSettingSubsystem.generated.h"


USTRUCT(BlueprintType)
struct FOtherSettings
{
	GENERATED_USTRUCT_BODY()

	FOtherSettings() {}

	UPROPERTY()
	bool bGuideBar = true;
	
};

UCLASS()
class ARCANEPUNK_API UAPSettingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UAPSettingSubsystem();

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

	FORCEINLINE const FOtherSettings & GetOtherSettings() {return OtherSettings;};
	FORCEINLINE void SetOtherSettings_GuideBar(bool NewBool) {OtherSettings.bGuideBar = NewBool;};

	FORCEINLINE TArray<bool> GetSmartKey() {return SmartKeyArr;};
	FORCEINLINE void SetSmartKey(TArray<bool> NewBools) {SmartKeyArr = NewBools;};

private:
	UPROPERTY()
	TArray<class APawn*> Players;
	UPROPERTY()
	uint8 PlayerNumber = 0;


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

	UPROPERTY()
	FOtherSettings OtherSettings;

	UPROPERTY()
	TArray<bool> SmartKeyArr; // QER 스마트키
};
