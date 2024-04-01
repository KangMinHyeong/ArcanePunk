// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APBattleCursor.generated.h"

class APlayerController;
class APawn;
class UCanvasPanel;

UCLASS()
class ARCANEPUNK_API UAPBattleCursor : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void CheckCursorUnder();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MainPanel;
};
