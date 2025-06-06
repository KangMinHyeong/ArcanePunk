// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APTutorialUI.generated.h"

class UBorder;
class APlayerController;
class UAPConversationUI;
class UTextBlock;
class AAPGameModeTutorialStage;

USTRUCT()
struct FTutorialData
{
	GENERATED_USTRUCT_BODY()

	FTutorialData() {}

	bool bMove_W = false;
	bool bMove_A = false;
	bool bMove_S = false;
	bool bMove_D = false;
	uint8 MoveTutorialPassNum = 0;

	uint8 DashTutorialPassNum = 0;

};

UCLASS()
class ARCANEPUNK_API UAPTutorialUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnFadeOut();
	UFUNCTION(BlueprintImplementableEvent)
	void OnFadeIn();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFadeIn_GuideUI();
	UFUNCTION(BlueprintImplementableEvent)
	void OnFadeOut_GuideUI();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFadeIn_TargetUI();
	UFUNCTION(BlueprintImplementableEvent)
	void OnFadeOut_TargetUI();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHighLight_Dash();

	UFUNCTION(BlueprintCallable)
	void OnSwitchingGuide();

	UFUNCTION()
	void OnNextGuide(int32 GroupID);

private:
	FReply StartMoveTutorial(const FReply & Reply, const FKeyEvent& InKeyEvent);
	void CheckDashNumber(const FKeyEvent& InKeyEvent);

	void StopPlayer();
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* FadeOutBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UWidget* HightLightUI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UTextBlock* Text_Target;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UAPConversationUI* GuideTextUI;

	int32 GuideOrder = 0;

	UPROPERTY()
	FTutorialData TutorialData;

	bool bMoveTutorial = false;
	bool bAttackTutorial = false;
	bool bDashTutorial = false;
		
	FTimerHandle TutorialTimerHandle;

	TWeakObjectPtr<APlayerController> OwnerPC;
	TWeakObjectPtr<AAPGameModeTutorialStage> TutorialGM;
};
