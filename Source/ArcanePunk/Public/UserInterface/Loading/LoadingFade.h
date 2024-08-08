// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingFade.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API ULoadingFade : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void FadeIn(float MultipleSpeed);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void FadeOut(float MultipleSpeed, bool bEntrance);

	UFUNCTION(BlueprintCallable)
	void OnEndedFade();
	
};
