// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitFade.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UHitFade : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void FadeOut();	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FadeTime = 2.5f;
};
