// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenu.h"
#include "GameFramework/HUD.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "APHUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UMainMenu;
class UAPTuTorialUserWidget;
class UAPStageInformationUI;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateHPBar, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateMPBar, uint8, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateSkillSlot, ESkillKey, uint8);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOperateSkill, ESkillKey);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartCoolTime, ESkillKey);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHightLightSkill, ESkillKey);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUsingSkill, ESkillKey, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCheckChargeNum, ESkillKey);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChargingGauge, float, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChargingEnd, bool);

UCLASS()
class ARCANEPUNK_API AAPHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> MainMenuClass;

	// UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	// TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UAPTuTorialUserWidget> TutorialWidgetClass;

	bool TutorialDone;

	bool bIsMenuVisible;

	AAPHUD();

	virtual void BeginPlay() override;

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	// void ShowInteractionWidget() const;
	// void HideInteractionWidget() const;
	// void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

	void UpdateTutorialWidget(const FString PressedKey);
	void HideTutorialWidget() const;

	UMainMenu* GetInventoryWidget() { return MainMenuWidget; };

	FVector2D GetViewportSize() { return ViewportSize; }

	// Minhyeong
	UFUNCTION(BlueprintPure)
	UUserWidget* GetBossHPUI() {return BossHPWidget.Get();};
	void SetBossHPUI();

	UFUNCTION(BlueprintPure)
	FORCEINLINE UUserWidget* GetStatusWidget() const {return StatusBarWidget.Get();};

	void DisplayEnhanceChoice(EEnhanceCategory EnhanceCategory, EEnHanceType UpdateEnHanceType, bool bOnlyEnhance = false);
	void DisplayEnhanceGauge(int32 TargetNum, int32 MaxNum);

	void OpenWorldMap();

protected:

	// ���� ���� ������

	UPROPERTY()
	UMainMenu* MainMenuWidget;

	// UPROPERTY()
	// UInteractionWidget* InteractionWidget;

	UPROPERTY()
	UAPTuTorialUserWidget* TutorialWidget;

	UPROPERTY()
	FVector2D ViewportSize;

//Minhyeong
private:
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> StatusBarUIClass;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> BossHPUIClass;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TArray<TSubclassOf<UUserWidget>> EnhanceChoiceClasses;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> EnhanceGaugeUIClass;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> StageInformationUIClass;

	TWeakObjectPtr<UUserWidget> StatusBarWidget;

	TWeakObjectPtr<UUserWidget> BossHPWidget;

	TWeakObjectPtr<UAPStageInformationUI> StageInformationUI;

public:
	FOnUpdateHPBar OnUpdateHPBar;

	FOnUpdateMPBar OnUpdateMPBar;

	FOnUpdateSkillSlot OnUpdateSkillSlot;

	FOnOperateSkill OnOperateSkill;

	FOnHightLightSkill OnHightLightSkill;

	FOnUsingSkill OnUsingSkill;

	FOnStartCoolTime OnStartCoolTime;

	FOnCheckChargeNum OnChargeTime;

	FOnChargingGauge OnChargingGauge;

	FOnChargingEnd OnChargingEnd;
};
