// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "APSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UAPSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RestoreConstant();
	void RestoreInterp();
	void ZoomConstant(float InOutLength, float ZoomTime);
	void ZoomInterpto(float InOutLength, float Speed);
	void ZoomImmediate(float InOutLength);
	// void ZoomOut(float ZoomTime);

private:
	bool bZoomConstant = false;
	bool bZoomInterp = false;

	float OriginLength;
	float TargetLength;

	float ZoomSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinimumSpringArmLength = 150.0f;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaximumSpringArmLength = 1450.0f;
};
