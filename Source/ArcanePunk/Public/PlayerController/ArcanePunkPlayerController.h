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
	void SetActivate_R(bool bValue);
	void Casting();
	void StartFadeIn();
	void StartFadeOut();
	void StartLoading();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	void MouseSkillEvent();

private:
	void LookStatus();
	void FreeCameraMode();
	void IsCasting(class AArcanePunkCharacter* APCharacter, FVector HitPoint);
	bool ViewInteraction(class AArcanePunkCharacter* APCharacter, float Distance, FVector HitPoint);

private:
	FTimerHandle LoadTimerHandle;

	bool bLookStatus = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> StatusWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> FadeLoadingWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoadingWidgetClass;

	UUserWidget* LoadingWidget;

	UPROPERTY(EditAnywhere)
	UUserWidget* StatusWidget;

	FInputModeGameOnly GameInputMode;

	FInputModeGameAndUI UIInputMode;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APawn> FreeCameraClass;

	bool bFreeCameraMode = false;

	APawn* FreeCamera;
	class AArcanePunkCharacter* MyCharacter;

	bool bCast = false;
	bool bActivate_R = false;
	bool bCanSkill_R = false;

	UPROPERTY(EditAnywhere)
	float LoadingTime = 2.0f;

};
