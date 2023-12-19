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
	UFUNCTION(BlueprintImplementableEvent)
	void FadeIn();

	UFUNCTION(BlueprintImplementableEvent)
	void FadeOut();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FadeTime = 2.0f;
	
};
