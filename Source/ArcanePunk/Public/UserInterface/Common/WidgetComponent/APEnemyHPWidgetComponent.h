// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APEnemyHPWidgetComponent.generated.h"

class UTextBlock;
class UProgressBar;
class AEnemy_CharacterBase;
class UAPShieldHPBar;

UCLASS()
class ARCANEPUNK_API UAPEnemyHPWidgetComponent : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetEnemy(AEnemy_CharacterBase* OwnerEnemy);
	void UpdateShieldHP(float ShieldHP);

private:
	void UpdateHealthPercentage(float InDeltaTime);
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HPPercent;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MaxHP;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* HP;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAPShieldHPBar* ShieldHPBar;

	UPROPERTY(EditAnywhere)
	float BarSpeed = 15.0f;

	TWeakObjectPtr<AEnemy_CharacterBase> Enemy;

	bool bActivate = false;

	float OriginHP = 0.0f;
	float OriginMaxHP = 0.0f;
};
