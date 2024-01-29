// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APSlotSwitcher.generated.h"

class USizeBox;
class UOverlay;
class UImage;

UCLASS()
class ARCANEPUNK_API UAPSlotSwitcher : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* Switcher_SizeBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Switcher_BackGround;
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* Switcher_Overlay;

};
