// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "APCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UAPCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void OnParryingCameraEffect();
	void Restore();

private:	
	UPROPERTY(EditAnywhere, Category = "Parrying")
	UMaterialInterface* ParryingGlitch;
	UPROPERTY(EditAnywhere, Category = "Parrying")
	UMaterialInterface* ParryingDistortion;
	UPROPERTY(EditAnywhere, Category = "Parrying")
	float ParryingGlitchValue = 0.0015f;
	UPROPERTY(EditAnywhere, Category = "Parrying")
	float ParryingColorContrast = 1.65f;
	UPROPERTY(EditAnywhere, Category = "Parrying")
	float ParryingIntensity = -0.4f;
	UPROPERTY(EditAnywhere, Category = "Parrying")
	float ParryingIntensitySpeed = 0.5f;

	TWeakObjectPtr<UMaterialInstanceDynamic> ParryingGlitchDynamic;
	TWeakObjectPtr<UMaterialInstanceDynamic> ParryingDistortionDynamic;

	bool bParrying = false;
	bool bColorContrast = false;
	float TargetIntensity = 0.0f;
	float TargetColorContrast = 1.0f;
};
