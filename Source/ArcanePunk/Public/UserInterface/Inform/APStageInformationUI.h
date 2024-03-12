// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APStageInformationUI.generated.h"

class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPStageInformationUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void UpdateRemainMonsterNumber();
	
protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_StageTitle;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_ChapterNumber;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_StageClearCondition;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_RemainMonsterNumber;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Text_TotalMonsterNumber;
};
