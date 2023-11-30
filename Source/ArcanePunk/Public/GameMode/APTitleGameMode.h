// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "APTitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AAPTitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AAPTitleGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;	
	
};
