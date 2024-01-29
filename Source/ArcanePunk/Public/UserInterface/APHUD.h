// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "APHUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UMainMenu;
class UAPTuTorialUserWidget;

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
	FORCEINLINE UUserWidget* GetSkillPressWidget() const {return SkillPressWidget;};

	void DisplayEnhanceChoice(ESkillKey EnhanceSkill, EEnHanceType UpdateEnHanceType);

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
	TSubclassOf<UUserWidget> SkillPressClass;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> BossHPUIClass;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TArray<TSubclassOf<UUserWidget>> EnhanceChoiceClasses;

	UPROPERTY()
	UUserWidget* SkillPressWidget;

	UPROPERTY()
	UUserWidget* BossHPWidget;


};
