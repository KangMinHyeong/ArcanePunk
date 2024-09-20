
#include "Components/Character/APCameraComponent.h"

void UAPCameraComponent::BeginPlay()
{
    Super::BeginPlay();

    ParryingGlitchDynamic = UMaterialInstanceDynamic::Create(ParryingGlitch, this);
    ParryingDistortionDynamic = UMaterialInstanceDynamic::Create(ParryingDistortion, this);

    AddOrUpdateBlendable(ParryingGlitchDynamic.Get(), 1.0f);
	AddOrUpdateBlendable(ParryingDistortionDynamic.Get(), 1.0f);
}

void UAPCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(bParrying)
    {
        float CurrentIntensity = ParryingDistortionDynamic->K2_GetScalarParameterValue(TEXT("Intensity"));
        CurrentIntensity = FMath::FInterpConstantTo(CurrentIntensity, TargetIntensity, DeltaTime, ParryingIntensitySpeed);
        ParryingDistortionDynamic->SetScalarParameterValue(TEXT("Intensity"), CurrentIntensity);

        if(abs(CurrentIntensity - TargetIntensity) <= 0.01f)
        bParrying = false;
    }

    if(bColorContrast)
    {
        float CurrentColorContrast = PostProcessSettings.ColorContrast.X;
        CurrentColorContrast = FMath::FInterpConstantTo(CurrentColorContrast, TargetColorContrast, DeltaTime, ParryingIntensitySpeed);
        PostProcessSettings.ColorContrast = FVector4(CurrentColorContrast, CurrentColorContrast, CurrentColorContrast, 1.0f);

        if(abs(CurrentColorContrast - TargetColorContrast) <= 0.01f)
        bColorContrast = false;
    }
}

void UAPCameraComponent::OnParryingCameraEffect()
{
    bParrying = true;
    bColorContrast = true;
    TargetIntensity = ParryingIntensity; 
    TargetColorContrast = ParryingColorContrast;
    ParryingGlitchDynamic->SetScalarParameterValue(TEXT("Red"), ParryingGlitchValue);
    ParryingGlitchDynamic->SetScalarParameterValue(TEXT("Green"), -ParryingGlitchValue);    
}

void UAPCameraComponent::Restore()
{
    bParrying = true;
    bColorContrast = true;
    TargetIntensity = 0.0f;
    TargetColorContrast = 1.0f;
    ParryingGlitchDynamic->SetScalarParameterValue(TEXT("Red"), 0.0f);
    ParryingGlitchDynamic->SetScalarParameterValue(TEXT("Green"), 0.0f);
}
