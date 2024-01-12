// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArcanePunkPlayerController.generated.h"

class UUserWidget;
class UAPStatusUI;
class APawn;
class AArcanePunkCharacter;
class UHomingTargetUI;

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

	void EnhanceChoiceMode(bool NewBool);

	void DisplayHomingUI(uint8 SkillNumber, uint8 SkillType);
	void ReturnToDefault();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	void LookStatus();
	void FreeCameraMode();
	void Setting();

private:
	FTimerHandle LoadTimerHandle;
	FTimerHandle SaveTimerHandle;

	bool bLookStatus = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPStatusUI> StatusWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> FadeLoadingWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SaveCompleteClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HitWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SmartKeySettingClass;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> HitCS;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHomingTargetUI> HomingUIClass;

	UUserWidget* LoadingWidget;

	UPROPERTY()
	UAPStatusUI* StatusWidget;
	
	UUserWidget* SaveUI;

	UPROPERTY()
	UHomingTargetUI* HomingUI;

	FInputModeGameOnly GameInputMode;

	FInputModeGameAndUI GameAndUIInputMode;

	FInputModeUIOnly UIInputMode;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> FreeCameraClass;

	bool bFreeCameraMode = false;

	APawn* FreeCamera;
	AArcanePunkCharacter* MyCharacter;

	UPROPERTY(EditAnywhere)
	float LoadingTime = 2.0f;

public:
	FOnUpdateStatusText OnUpdateStatusText;

	UPROPERTY()
	TArray<bool> SmartKeyArr; // QER 스마트키
};
