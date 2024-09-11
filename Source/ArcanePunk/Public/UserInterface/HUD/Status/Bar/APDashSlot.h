// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/HUD/Status/Bar/APSkillSlot.h"
#include "APDashSlot.generated.h"

class UProgressBar;

UCLASS()
class ARCANEPUNK_API UAPDashSlot : public UAPSkillSlot
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void StartDashTime(float DashTime);

private:
	void EndDash();
	float GetCurrentDashTime();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* Skilling_ProgressBar;

	bool bDashing = false;
	FTimerHandle DashTimerHandle;
	float CurrentDashTime = 1.0f;
};
