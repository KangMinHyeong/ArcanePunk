// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnhanceGauge.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class ARCANEPUNK_API UEnhanceGauge : public UUserWidget
{
	GENERATED_BODY()
public:
	void DisplayEnhanceGauge(int32 TargetNum, int32 MaxNum);

	UFUNCTION(BlueprintImplementableEvent)
	void OnGaugeDisplay();
	UFUNCTION(BlueprintImplementableEvent)
	void OnCompleteDisplay();


	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) override;
private:
	void DisplayCompleteText();
	void SetCustomTickEnable();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Gauge_ProgressBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Target_Text;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Max_Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bTickable = false;
	bool bCloseOn = false;

	float Current = 0.0f;
	float Target = 1.0f;
	UPROPERTY(EditAnywhere)
	float Speed = 100.0f;
	UPROPERTY(EditAnywhere)
	float Delay = 0.5f;
	UPROPERTY(EditAnywhere)
	float FadeoutDelay = 1.5f;

	int32 Target_Number = 1;
	int32 Max_Number = 1;
	
	FTimerHandle TimerHandle;

};
