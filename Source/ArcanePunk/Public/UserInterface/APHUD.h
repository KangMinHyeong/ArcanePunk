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
class UImitatorSkillSlot;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateMaxHPBar, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateHPBar, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateMaxMPBar, uint8);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateMPBar, uint8, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateSkillSlot, ESkillKey, uint8);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOperateSkill, ESkillKey); // 스킬 사용 UI 애니메이션
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartCoolTime, ESkillKey); // 스킬 쿨타임 시작
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHightLightSkill, ESkillKey); // 스킬 하이라이트 UI 애니
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUsingSkill, ESkillKey, bool); // 스킬 시전 중 

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCheckChargeNum, ESkillKey); // 스킬 슬롯 충전 횟수 쿨타임 체크
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOnlyChargingNumberChange, ESkillKey); // 스킬 슬롯 충전 횟수 텍스트 변경

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChargingGauge, float, float);  // 차징 스킬 게이지
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChargingEnd, bool); // 차징 스킬 시작 of 끝

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

	void DisplayEnhanceChoice(EEnhanceCategory EnhanceCategory, EEnHanceType UpdateEnHanceType, bool bOnlyEnhance = false, uint8 EnhanceSkillNum = 0);
	void DisplayEnhanceGauge(int32 TargetNum, int32 MaxNum);

	void OpenWorldMap();

	void StartImitatorSlot();
	void EndImitatorSlot();
	void UpdateImitatorSlot(uint8 SkillNumber = 0);

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
	TSubclassOf<UUserWidget> EnhanceChoiceClass;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> EnhanceGaugeUIClass;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> StageInformationUIClass;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> ImitatorSlotUIClass;

	TWeakObjectPtr<UUserWidget> StatusBarWidget;

	TWeakObjectPtr<UUserWidget> BossHPWidget;

	TWeakObjectPtr<UAPStageInformationUI> StageInformationUI;

	TWeakObjectPtr<UImitatorSkillSlot> ImitatorSlotUI;

public:
	FOnUpdateMaxHPBar OnUpdateMaxHPBar;

	FOnUpdateHPBar OnUpdateHPBar;

	FOnUpdateMaxMPBar OnUpdateMaxMPBar;

	FOnUpdateMPBar OnUpdateMPBar;

	FOnUpdateSkillSlot OnUpdateSkillSlot;

	FOnOperateSkill OnOperateSkill;

	FOnHightLightSkill OnHightLightSkill;

	FOnUsingSkill OnUsingSkill;

	FOnStartCoolTime OnStartCoolTime;

	FOnCheckChargeNum OnChargeTime;

	FOnOnlyChargingNumberChange OnOnlyChargingNumberChange;

	FOnChargingGauge OnChargingGauge;

	FOnChargingEnd OnChargingEnd;

};
