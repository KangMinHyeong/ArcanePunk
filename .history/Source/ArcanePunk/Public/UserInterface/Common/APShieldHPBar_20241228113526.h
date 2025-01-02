// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APShieldHPBar.generated.h"

class UProgressBar;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API UAPShieldHPBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void CreateShieldHPBar(AActor* Owner);
	void BreakShieldHPBar();

	UFUNCTION()
	void CheckShieldHP(AActor* Owner);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroyShield();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* ShieldHPBar;	

	TWeakObjectPtr<UNiagaraComponent> ShieldEffectComp;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ShieldEffect;
};
