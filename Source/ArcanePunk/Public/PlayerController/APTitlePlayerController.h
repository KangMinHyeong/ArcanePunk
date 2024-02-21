// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "APTitlePlayerController.generated.h"

class UUserWidget;
class UAPOptionSetting;

UCLASS()
class ARCANEPUNK_API AAPTitlePlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	void CreateTitleUI();
	void CreateIntroUI();

	void OptionSetting();

private:
	void RemoveIntroUI(UUserWidget* IntroWidget);

private:
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> TitleUIClass;

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> IntroUIClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OptionSettingClass;

	UPROPERTY()
	UAPOptionSetting* OptionSettingUI;

	FTimerHandle IntroeTimerHandle;

	UPROPERTY(EditAnywhere)
	float IntroTime = 5.0f;
};
