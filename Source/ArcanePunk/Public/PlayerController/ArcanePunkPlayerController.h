// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "GameFramework/PlayerController.h"
#include "ArcanePunkPlayerController.generated.h"

class UUserWidget;
class UAPStatusUI;
class UHomingTargetUI;
class UAPOptionSetting;
class UAPMouseClickBase;
class UAPSaveSlotUI;

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

	// Stage Enter UI
	void CreateEntranceUI();

	// Save UI
	void OpenSaveSlot();
	void CloseSaveSlot();

	void StartSaveUI();

	// Stage Selecting UI
	void OpenStageSelectingUI();
	void CloseStageSelectingUI();
	
	// Hit UI
	void HitUI();

	FORCEINLINE UAPStatusUI* GetStatusUI() { return StatusWidget; };

	void DisplayHomingUI(ESkillNumber SkillNumber);
	void ReturnToDefault();
	void PreventOtherClick(ESkillNumber SkillNumber);

	void SetHideUI(bool NewBool);

	// Option Menu
	void OptionSetting();

	UAPOptionSetting* GetOptionSettingUI() {return OptionSettingUI;};

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	void LookStatus();
	void FreeCameraMode();

	void EndSaveUI();

private:
	FTimerHandle SaveTimerHandle;

	bool bLookStatus = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPStatusUI> StatusWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> FadeLoadingWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> EntranceUIClass;

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

	// Save
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SelectSaveSlotClass;

	UPROPERTY()
	UAPSaveSlotUI* SelectSlotUI;

	// Option Setting 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OptionSettingClass;

	UPROPERTY()
	UAPOptionSetting* OptionSettingUI;

	// Stage Select 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> StageSelectingUIClasss;

public:
	FOnUpdateStatusText OnUpdateStatusText;

	UPROPERTY()
	TArray<bool> SmartKeyArr; // QER 스마트키
};
