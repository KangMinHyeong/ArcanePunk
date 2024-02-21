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

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateHPBar, float);

UCLASS()
class ARCANEPUNK_API AAPHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UAPTuTorialUserWidget> TutorialWidgetClass;

	bool TutorialDone;

	bool bIsMenuVisible;

	AAPHUD();


	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

	void UpdateTutorialWidget(const FString PressedKey);
	void HideTutorialWidget() const;

	// Minhyeong
	UFUNCTION(BlueprintPure)
	UUserWidget* GetBossHPUI() {return BossHPWidget;};
	void SetBossHPUI();

	UFUNCTION(BlueprintPure)
	FORCEINLINE UUserWidget* GetStatusWidget() const {return StatusBarWidget;};

	void DisplayEnhanceChoice(EEnhanceCategory EnhanceCategory, EEnHanceType UpdateEnHanceType);

protected:

	// ���� ���� ������

	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	UPROPERTY()
	UAPTuTorialUserWidget* TutorialWidget;


	virtual void BeginPlay() override;

//Minhyeong
private:
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> StatusBarUIClass;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> BossHPUIClass;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TArray<TSubclassOf<UUserWidget>> EnhanceChoiceClasses;

	UPROPERTY()
	UUserWidget* StatusBarWidget;

	UPROPERTY()
	UUserWidget* BossHPWidget;

	UPROPERTY()
	FVector2D ViewportSize;

public:

	UMainMenu* GetInventoryWidget() { return MainMenuWidget; };

	FVector2D GetViewportSize() { return ViewportSize; }



public:
	FOnUpdateHPBar OnUpdateHPBar;
};
