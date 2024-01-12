// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APStatusText.generated.h"

class UTextBlock;

UCLASS()
class ARCANEPUNK_API UAPStatusText : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	void UpdatePlayerStatus();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CharacterIndex;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CharacterName;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CharacterGroup;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Gender;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CharacterRace;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CharacterType;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* HP;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MaxHP;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MP;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MaxMP;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ATK;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ATKSpeed;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* DEF;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MoveSpeed;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SP;
};
