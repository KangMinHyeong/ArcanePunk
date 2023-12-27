// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArcanePunkPlayerController.generated.h"

class UUserWidget;
class UAPStatusUI;
class APawn;
class AArcanePunkCharacter;

DECLARE_MULTICAST_DELEGATE(FOnUpdateStatusText);

UCLASS()
class ARCANEPUNK_API AArcanePunkPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AArcanePunkPlayerController();
	void SetActivate_Skill3(bool bValue);
	void Casting();
	void StartFadeIn();
	void StartFadeOut();
	void StartLoading();
	void StartSaveUI();
	void EndSaveUI();

	void HitUI();

	FORCEINLINE UAPStatusUI* GetStatusUI() { return StatusWidget; };

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	void MouseSkillEvent();

private:
	void LookStatus();
	void FreeCameraMode();
	void IsCasting(AArcanePunkCharacter* APCharacter, FVector HitPoint);
	bool ViewInteraction(AArcanePunkCharacter* APCharacter, float Distance, FVector HitPoint);

private:
	FTimerHandle LoadTimerHandle;
	FTimerHandle SaveTimerHandle;

	bool bLookStatus = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPStatusUI> StatusWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> FadeLoadingWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SaveCompleteClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HitWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> HitCS;

	UUserWidget* LoadingWidget;

	UPROPERTY()
	UAPStatusUI* StatusWidget;
	
	UUserWidget* SaveUI;

	FInputModeGameOnly GameInputMode;

	FInputModeGameAndUI UIInputMode;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> FreeCameraClass;

	bool bFreeCameraMode = false;

	APawn* FreeCamera;
	AArcanePunkCharacter* MyCharacter;

	bool bCast = false;
	bool bActivate_R = false;
	bool bCanSkill_R = false;

	UPROPERTY(EditAnywhere)
	float LoadingTime = 2.0f;

public:
	FOnUpdateStatusText OnUpdateStatusText;
};
