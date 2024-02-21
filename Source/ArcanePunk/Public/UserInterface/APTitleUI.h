// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APTitleUI.generated.h"

class UAPSaveSlotUI;
class UButton;

UCLASS()
class ARCANEPUNK_API UAPTitleUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void NewGame();

	UFUNCTION()
	void Continue();

	UFUNCTION()
	void Setting();

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_NewGame;

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Continue;	

	UPROPERTY(EditAnywhere, meta = (BindWidget))	
	UButton* Button_Setting;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPSaveSlotUI> SelectSaveSlotClass;
};
