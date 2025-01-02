// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APShieldHPBar.generated.h"

class UProgressBar;
class UNiagaraSystem;
class UNiagaraComponent;
class AAPCharacterBase;

UCLASS()
class ARCANEPUNK_API UAPShieldHPBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void CreateShieldHPBar();
	void BreakShieldHPBar();

	UFUNCTION()
	void CheckShieldHP(AActor* Owner, float OriginShieldHP);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroyShield();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* ShieldHPBar;	

	TWeakObjectPtr<UNiagaraComponent> ShieldEffectComp;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ShieldEffect;

	UPROPERTY(EditAnywhere)
	float ShieldPercentSpeed = 66.0f;

	float CurrentOriginShieldHP = 0.0f;

	bool bChange = false;
	TWeakObjectPtr<AAPCharacterBase> OwnerCharacter;
};
