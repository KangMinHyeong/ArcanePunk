// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "GameFramework/PlayerController.h"
#include "ArcanePunkPlayerController.generated.h"

class UUserWidget;
class UAPStatusUI;
class UHomingTargetUI;
class UAPOptionSetting;
class UAPMouseClickBase;
class UAPSaveSlotUI;
class UInteractionWidget;
struct FInteractData;
struct FConversationData;
class UAPConversationUI;
class UAPShoppingUI;
class UAPSkillWindow;

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

	void SetMouseCursor();
	
	// Stage Enter UI
	void CreateEntranceUI();

	// Save UI
	void OpenSaveSlot();
	void CloseSaveSlot();

	void StartSaveUI();

	// Stage Selecting UI
	void OpenStageSelectingUI(AActor* CameraActor);
	void CloseStageSelectingUI();
	
	// Conversation UI
	void OpenConversationUI(AActor* CameraActor, FName Name, uint8 State);
	void CloseConversationUI();

	// Shopping UI
	void OpenShoppingUI(AActor* ShopActor, FShopListData ShopListData);
	void CloseShoppingUI();
	
	// Hit UI
	void HitUI();

	FORCEINLINE UAPStatusUI* GetStatusUI() { return StatusWidget.Get(); };

	void DisplayHomingUI(ESkillNumber SkillNumber);
	void ReturnToDefault();
	void PreventOtherClick(ESkillNumber SkillNumber, bool Loop = false);
	void PreventOtherClick_Ult( bool Loop = false);
	void RemoveOtherClick();
	
	// Arcane Tent
	void SetHideUI(bool NewBool);

	// Option Menu
	void OptionSetting();

	// Interaction UI
	void OpenInteraction(AActor* NewActor, FInteractData InteractData);
	void CloseInteraction(AActor* NewActor);

	// Dead UI
	void DisplayDeadUI();

	// Not Enough MP
	void DisplayNotEnoughMPUI();

	// Skill Window
	void DisplaySkillWindow();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	void LookStatus();
	void FreeCameraMode();

	//Interaction
	void OnStageSelectingUI();
	void OnConversationUI(FName Name, uint8 State);

private:
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DeadUIClass;

	TWeakObjectPtr<UAPStatusUI> StatusWidget;
	
	TWeakObjectPtr<UUserWidget> SaveUI;

	TWeakObjectPtr<UHomingTargetUI> HomingUI;
	TWeakObjectPtr<UAPMouseClickBase> MouseClickUI;

	TWeakObjectPtr<UUserWidget> HideUI;

	FInputModeGameOnly GameInputMode;
	FInputModeGameAndUI GameAndUIInputMode;
	FInputModeUIOnly UIInputMode;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> FreeCameraClass;

	bool bFreeCameraMode = false;

	TWeakObjectPtr<APawn> FreeCamera;
	TWeakObjectPtr<AArcanePunkCharacter> MyCharacter;

	// Loading
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	TWeakObjectPtr<UUserWidget> LoadingWidget;

	FTimerHandle LoadTimerHandle;

	UPROPERTY(EditAnywhere)
	float LoadingTime = 2.0f;

	// Save
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SelectSaveSlotClass;

	TWeakObjectPtr<UAPSaveSlotUI> SelectSlotUI;

	// Option Setting 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OptionSettingClass;

	// Stage Select 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> StageSelectingUIClasss;

	// Interaction UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InteractionUIClass;

	TWeakObjectPtr<UInteractionWidget> InteractionWidget;

	TWeakObjectPtr<AActor> InteractionActor;

	FTimerHandle InteractionTimerHandle;

	UPROPERTY(EditAnywhere)
	float BlendTime = 3.0f;

	// Conversation UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ConversationUIClass;
	
	TWeakObjectPtr<UAPConversationUI> ConversationUI;

	// Conversation UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ShoppingUIClass;
	TWeakObjectPtr<UAPShoppingUI> ShoppingUI;

	// Not Enough MP
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> NotEnoughMPUIClass;
	
	// SkillWindowUIClass 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SkillWindowUIClass;

public:
	FOnUpdateStatusText OnUpdateStatusText;

	UPROPERTY()
	TArray<bool> SmartKeyArr; // QER 스마트키
	
	float MousePositionAngle = 0.0f;
};
