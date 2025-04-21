// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "GameFramework/PlayerController.h"
#include "ArcanePunkPlayerController.generated.h"

class UUserWidget;
class UAPStatusUI;
class UAPOptionSetting;
class UAPSaveSlotUI;
class UInteractionWidget;
struct FInteractData;
struct FConversationData;
class UAPConversationUI;
class UAPShoppingUI;
class UAPSkillWindow;
class ULoadingFade;
class UNiagaraSystem;

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

	FORCEINLINE APawn* GetMainPlayer() const {return Players[MainPlayerIndex];};
	FORCEINLINE int32 GetPlayerNumbers() const {return Players.Num();};

	void InitMainPlayer(AActor* PlayerStart);
	FORCEINLINE void SwapPlayer_0() {SwapMainPlayerByIndex(0);};
	FORCEINLINE void SwapPlayer_1() {SwapMainPlayerByIndex(1);};
	FORCEINLINE void SwapPlayer_2() {SwapMainPlayerByIndex(2);};
	void SwapMainPlayerByIndex(uint8 Index);

	void StartFadeIn(float MultipleSpeed = 1.0f, bool bEntrance = true);
	void StartFadeOut(float MultipleSpeed = 1.0f, bool bEntrance = true);
	void StartLoading();	

	void SetMouseCursor();
	
	// Save UI
	void OpenSaveSlot();
	void CloseSaveSlot();

	// Stage Selecting UI
	void OpenStageSelectingUI(AActor* CameraActor);
	void CloseStageSelectingUI();
	
	// Conversation UI
	void OpenConversationUI(AActor* CameraActor, const int32 GroupID);
	UFUNCTION()
	void CloseConversationUI(const int32 GroupID);

	// Shopping UI
	void OpenShoppingUI(AActor* ShopActor, const FShopListData & ShopListData);
	void CloseShoppingUI();
	
	// Hit UI
	void HitUI();

	FORCEINLINE UAPStatusUI* GetStatusUI() { return StatusWidget.Get(); };

	void ReturnToDefault();
	
	// Arcane Tent
	void SetHideUI(bool NewBool);

	// Option Menu
	void OptionSetting();

	// Dead UI
	void DisplayDeadUI();

	// Skill Window
	void DisplaySkillWindow();

	// Parrying CameraShake
	void ParryingCameraShake();

	// Attack CameraShake
	void AttackCameraShake();

	// DisplayEnding
	void DisplayEnding();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

	void SwapPlayer(const & FVector Loc, const & FRotator Rot);
	bool CheckSwapPlayerCondition(uint8 Index);
	AActor* DetectEnemy(AArcanePunkCharacter* CurrentPlayer);

private:
	void LookStatus();

	//Interaction
	void OnStageSelectingUI();
	void OnConversationUI(const int32 GroupID);

private:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APawn>> PlayerClass;
	UPROPERTY()
	TArray<APawn*> Players;
	TWeakObjectPtr<AArcanePunkCharacter> MyCharacter;
	
	bool bLookStatus = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPStatusUI> StatusWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> FadeLoadingWidgetClass;

	TWeakObjectPtr<ULoadingFade> FadeLoadingWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HitWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ParryScreenEffectClass;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> HitCS;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> ParryingCS;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> AttackCS;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HideUIClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DeadUIClass;

	TWeakObjectPtr<UAPStatusUI> StatusWidget;
	
	TWeakObjectPtr<UUserWidget> SaveUI;

	TWeakObjectPtr<UUserWidget> HideUI;

	FInputModeGameOnly GameInputMode;
	FInputModeGameAndUI GameAndUIInputMode;
	FInputModeUIOnly UIInputMode;

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

	TWeakObjectPtr<UAPOptionSetting> OptionSettingUI;

	// Stage Select 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> StageSelectingUIClasss;

	FTimerHandle InteractionTimerHandle;

	UPROPERTY(EditAnywhere)
	float BlendTime = 3.0f;

	FTimerHandle SwapTimerHandle;

	UPROPERTY(EditAnywhere)
	float SwapDelayTime = 0.75f;
	
	float SwapSlow = 1.0f;
	float SwapSlowTime = 0.0f;

	bool bSwap = false;
	bool bEnhanceSwap = false;
	AActor* SwapEnemy = nullptr;

	// Conversation UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ConversationUIClass;
	
	TWeakObjectPtr<UAPConversationUI> ConversationUI;

	// Conversation UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ShoppingUIClass;
	TWeakObjectPtr<UAPShoppingUI> ShoppingUI;
	
	// SkillWindowUIClass 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SkillWindowUIClass;

	//Demo Ending UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DemoEndUIClass;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SwitchingEffect;

	UPROPERTY()
	AActor* ConversationActor;

	UPROPERTY(EditAnywhere)
	FName CannotChange = "";

	
public:
	FOnUpdateStatusText OnUpdateStatusText;
	
	float MousePositionAngle = 0.0f;

	uint8 MainPlayerIndex = 0;
};
