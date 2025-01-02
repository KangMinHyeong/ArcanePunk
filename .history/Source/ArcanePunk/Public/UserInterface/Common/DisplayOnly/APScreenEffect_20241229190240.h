// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APScreenEffect.generated.h"


UCLASS()
class ARCANEPUNK_API UAPScreenEffect : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnScreenEffect();

};
