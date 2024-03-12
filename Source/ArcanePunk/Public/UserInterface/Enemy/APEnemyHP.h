// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APEnemyHP.generated.h"

class UTextBlock;
class UProgressBar;
class AEnemy_CharacterBase;

UCLASS()
class ARCANEPUNK_API UAPEnemyHP : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetEnemy(AEnemy_CharacterBase* OwnerEnemy);

private:
	void GetHealthPercentage();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HPPercent;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MaxHP;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* HP;

	TWeakObjectPtr<AEnemy_CharacterBase> Enemy;
};
