// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APTuTorialUserWidget.generated.h"

class UImage;
class UTextBlock;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API UAPTuTorialUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	bool UpdateWidget(const FString PressedKey);

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | Input")
	UTextBlock* KeyPressText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | Description")
		UImage* Example;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* W;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* S;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* A;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* D;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* Shift;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* Q;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* E;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* R;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* F;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* Tab;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* Ctrl;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Tutorial Widget | key")
	UImage* Nine;

	UPROPERTY(VisibleAnyWhere);
	TArray<UImage*> Keys;

	UPROPERTY(VisibleAnyWhere);
	TArray<FString> NeedInput;

	int32 NowIndex;

	float Time;

	void WidgetBlink();
};
