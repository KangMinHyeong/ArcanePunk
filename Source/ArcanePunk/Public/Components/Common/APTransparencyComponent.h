// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APTransparencyComponent.generated.h"

class UMaterialInstanceDynamic;

UCLASS()
class ARCANEPUNK_API AAPTransparencyComponent : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPTransparencyComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void FadeOut();
	void FadeIn();
private:
	bool bFadeOut = false;


};
