// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APTutorialUI.generated.h"

class UBorder;
class APlayerController;

UCLASS()
class ARCANEPUNK_API UAPTutorialUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) override;

	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnFadeOut();
	UFUNCTION(BlueprintImplementableEvent)
	void OnFadeIn();

	UFUNCTION(BlueprintCallable)
	void OnPlayingGuide_1();
	UFUNCTION(BlueprintCallable)
	void OnPlayingGuide_2();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* FadeOutBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UWidget* HightLightUI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UAPConversationUI* GuideConversationUI;

	TWeakObjectPtr<APlayerController> OwnerPC;
};