// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArcanePunkPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AArcanePunkPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AArcanePunkPlayerController();

	virtual void SetupInputComponent() override;

private:
	void LookStatus();

private:
	bool bLookStatus = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> StatusWidgetClass;

	UPROPERTY(EditAnywhere)
	UUserWidget* StatusWidget;

	FInputModeGameOnly GameInputMode;

	FInputModeGameAndUI UIInputMode;

};
