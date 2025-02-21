// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/Inventory/MainMenu.h"
#include "GameFramework/HUD.h"
#include "UserInterface/HUD/Status/Bar/APStatusBar.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "APHUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UMainMenu;
class UAPTuTorialUserWidget;
class UAPStageInformationUI;
class UImitatorSkillSlot;
class AAPGameModeBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateMaxHPBar, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateHPBar, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateMaxMPBar, uint8);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateMPBar, uint8, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateSkillSlot, ESkillKey, uint8);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOperateSkill, ESkillKey); // 스킬 사용 UI 애니메이션
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStartCoolTime, ESkillKey, float); // 스킬 쿨타임 시작
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHightLightSkill, ESkillKey); // 스킬 하이라이트 UI 애니
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUsingSkill, ESkillKey, bool); // 스킬 시전 중 
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCheckChargeNum, ESkillKey); // 스킬 슬롯 충전 횟수 쿨타임 체크
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOnlyChargingNumberChange, ESkillKey); // 스킬 슬롯 충전 횟수 텍스트 변경
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChargingGauge, float, float);  // 차징 스킬 게이지
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChargingEnd, bool); // 차징 스킬 시작 of 끝
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAddingCoolTime, ESkillKey, float); // 쿨타임 빼기
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDashTime, float); // 쿨타임 빼기
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHiddenGuideBar, bool); // 쿨타임 빼기

UCLASS()
class ARCANEPUNK_API AAPHUD : public AHUD
{
	GENERATED_BODY()

public:
	AAPHUD();

protected:
	virtual void BeginPlay() override;

public:
	void Init();
	void BindStatusBar(AArcanePunkCharacter* OwnerPlayer, uint8 Index);
	void ActivateStatusBar();
	
	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	UMainMenu* GetInventoryWidget() { return MainMenuWidget; };
	FVector2D GetViewportSize() { return ViewportSize; }

	// Minhyeong
	UFUNCTION(BlueprintPure)
	UUserWidget* GetBossHPUI() {return BossHPWidget.Get();};
	void SetBossHPUI();

	UFUNCTION(BlueprintPure)
	UAPStatusBar* GetStatusWidget() const;

	void DisplayEnhanceChoice(EEnhanceCategory EnhanceCategory, EEnHanceType UpdateEnHanceType, bool bOnlyEnhance = false, uint8 EnhanceSkillNum = 0);
	void DisplayEnhanceGauge(int32 TargetNum, int32 MaxNum);

	void OpenWorldMap();
	void SetStatusVisibility(bool bHide);
	// void StartImitatorSlot();
	// void EndImitatorSlot();
	// void UpdateImitatorSlot(uint8 SkillNumber = 0);
	
protected:
	UPROPERTY()
	UMainMenu* MainMenuWidget;
	UPROPERTY()
	FVector2D ViewportSize;

	bool bIsMenuVisible;

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

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY()
	TArray<UAPStatusBar*> StatusBarWidgets;

	TWeakObjectPtr<AAPGameModeBase> GM;
	TWeakObjectPtr<UUserWidget> BossHPWidget;
	TWeakObjectPtr<UAPStageInformationUI> StageInformationUI;
	TWeakObjectPtr<UImitatorSkillSlot> ImitatorSlotUI;

public:
	// FOnUpdateMaxHPBar OnUpdateMaxHPBar;
	// FOnUpdateHPBar OnUpdateHPBar;
	// FOnUpdateMaxMPBar OnUpdateMaxMPBar;
	// FOnUpdateMPBar OnUpdateMPBar;
	// FOnUpdateSkillSlot OnUpdateSkillSlot;
	// FOnOperateSkill OnOperateSkill;
	// FOnHightLightSkill OnHightLightSkill;
	// FOnUsingSkill OnUsingSkill;
	// FOnStartCoolTime OnStartCoolTime;
	// FOnCheckChargeNum OnChargeTime;
	// FOnOnlyChargingNumberChange OnOnlyChargingNumberChange;
	// FOnChargingGauge OnChargingGauge;
	// FOnChargingEnd OnChargingEnd;
	// FOnAddingCoolTime OnAddingCoolTime;
	// FOnDashTime OnDashTime;
	// FOnHiddenGuideBar OnHiddenGuideBar;
};
