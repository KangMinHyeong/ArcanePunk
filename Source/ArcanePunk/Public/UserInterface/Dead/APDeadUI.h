// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APDeadUI.generated.h"

class UButton;

UCLASS()
class ARCANEPUNK_API UAPDeadUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnChoice_TownButton();

	UFUNCTION()
	void OnChoice_TitleButton();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Town_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Title_Button;

	UPROPERTY(EditAnywhere)
	FName LevelName_Town;

	UPROPERTY(EditAnywhere)
	FName LevelName_Title;
};
