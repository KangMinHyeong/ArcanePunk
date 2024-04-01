// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APFootPrint.generated.h"

class UDecalComponent;

UCLASS()
class ARCANEPUNK_API AAPFootPrint : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPFootPrint();

protected:
	virtual void BeginPlay() override;
	void FadeOut();

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	UDecalComponent* FootDecal;

	UPROPERTY(EditAnywhere, Category = Decal)
	float Opacity = 5.0f;

	UPROPERTY(EditAnywhere, Category = Decal)
	float FadeStartDelay = 1.5f;

	UPROPERTY(EditAnywhere, Category = Decal)
	float FadeOutTime = 3.5f;

	UPROPERTY(EditAnywhere, Category = Decal)
	float FadeOutSpeed = 2.0f;

	FTimerHandle FadeTimerHandle;

};
