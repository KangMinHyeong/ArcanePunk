// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APOptionSetting.generated.h"

class UButton;

UCLASS()
class ARCANEPUNK_API UAPOptionSetting : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

	void BindButton();

	UFUNCTION()
	void OnClickButton_Graphics();
	UFUNCTION()
	void OnClickButton_Audio();
	UFUNCTION()
	void OnClickButton_Key();

	UFUNCTION()
	void OnClickButton_Back();

private:
//  Frame Rate Limit
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Graphics;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Audio;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Key;



	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Back;

};
