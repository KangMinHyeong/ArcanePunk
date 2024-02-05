// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/Setting/APGraphicsSetting.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "GameFramework/PlayerController.h"
#include "ArcanePunkPlayerController.generated.h"

class UUserWidget;
class UAPStatusUI;
class UHomingTargetUI;
class UAPMouseClickBase;
class UAPAudioSetting;

DECLARE_MULTICAST_DELEGATE(FOnUpdateStatusText);

UENUM()
enum class ECursorType : uint8
{
	Default		= 0,
	Crosshairs	= 1,
};

UCLASS()
class ARCANEPUNK_API AArcanePunkPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AArcanePunkPlayerController();

	void StartFadeIn();
	void StartFadeOut();
	void StartLoading();
	void StartSaveUI();
	void EndSaveUI();

	void HitUI();

	FORCEINLINE UAPStatusUI* GetStatusUI() { return StatusWidget; };

	void DisplayHomingUI(ESkillNumber SkillNumber);
	void ReturnToDefault();
	void PreventOtherClick(ESkillNumber SkillNumber);

	void SetHideUI(bool NewBool);

	// Option Setting
	void InitGraphicsSetting();
	void InitAudioSetting();

	void OptionSetting();

	void SmartKeySetting();
	void GraphicsSetting();
	void AudioSetting();

	FORCEINLINE float GetMasterVolume() const { return MasterVolume;};
	FORCEINLINE float GetBGMVolume() const { return BGMVolume;};
	FORCEINLINE float GetEffectVolume() const { return EffectVolume;};

	FORCEINLINE void SetMasterVolume(float NewValue) { MasterVolume = NewValue;};
	FORCEINLINE void SetBGMVolume(float NewValue) { BGMVolume = NewValue;};
	FORCEINLINE void SetEffectVolume(float NewValue) { EffectVolume = NewValue;};

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	void LookStatus();
	void FreeCameraMode();

private:
	FTimerHandle SaveTimerHandle;

	bool bLookStatus = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPStatusUI> StatusWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> FadeLoadingWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SaveCompleteClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HitWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> HitCS;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHomingTargetUI> HomingUIClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPMouseClickBase> MouseClickUIClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HideUIClass;

	UPROPERTY()
	UAPStatusUI* StatusWidget;
	
	UPROPERTY()
	UUserWidget* SaveUI;

	UPROPERTY()
	UHomingTargetUI* HomingUI;
	UPROPERTY()
	UAPMouseClickBase* MouseClickUI;

	UPROPERTY()
	UUserWidget* HideUI;

	FInputModeGameOnly GameInputMode;
	FInputModeGameAndUI GameAndUIInputMode;
	FInputModeUIOnly UIInputMode;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> FreeCameraClass;

	bool bFreeCameraMode = false;

	UPROPERTY()
	APawn* FreeCamera;
	UPROPERTY()
	AArcanePunkCharacter* MyCharacter;

	// Loading
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY()
	UUserWidget* LoadingWidget;

	FTimerHandle LoadTimerHandle;

	UPROPERTY(EditAnywhere)
	float LoadingTime = 2.0f;

	// Option Setting 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OptionSettingClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GraphicsSettingClass; // Graphics

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> AudioSettingClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SmartKeySettingClass;

	UPROPERTY(EditAnywhere)
	TMap<FName, EGraphicsSetting> DefaultGraphicsSetting; // Graphics

	UPROPERTY()
	UAPGraphicsSetting* GraphicsSettingUI;

	UPROPERTY()
	UAPAudioSetting* AudioSettingUI;
	UPROPERTY(EditAnywhere)
	float MasterVolume = 1.0f;
	UPROPERTY(EditAnywhere)
	float BGMVolume = 1.0f;
	UPROPERTY(EditAnywhere)
	float EffectVolume = 1.0f;

public:
	FOnUpdateStatusText OnUpdateStatusText;

	UPROPERTY()
	TArray<bool> SmartKeyArr; // QER 스마트키
};
