
#include "Components/Character/APSpringArmComponent.h"

void UAPSpringArmComponent::BeginPlay()
{
    Super::BeginPlay();
    OriginLength = TargetArmLength;
}

void UAPSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(bZoomConstant)
    {
        TargetArmLength = FMath::FInterpConstantTo(TargetArmLength, TargetLength, DeltaTime, ZoomSpeed);
        TargetArmLength = FMath::Clamp(TargetArmLength, MinimumSpringArmLength, MaximumSpringArmLength);
        
        if(abs(TargetLength - TargetArmLength) <= KINDA_SMALL_NUMBER) bZoomConstant = false;
    }
    else if(bZoomInterp)
    {
        TargetArmLength = FMath::FInterpTo(TargetArmLength, TargetLength, DeltaTime, ZoomSpeed);
        TargetArmLength = FMath::Clamp(TargetArmLength, MinimumSpringArmLength, MaximumSpringArmLength);

        if(abs(TargetLength - TargetArmLength) <= 4.0f)
        {
            TargetArmLength = TargetLength;
            bZoomInterp = false;
        }
    }
}

void UAPSpringArmComponent::RestoreConstant()
{
    bZoomConstant = true;
    TargetLength = OriginLength;
}

void UAPSpringArmComponent::RestoreInterp()
{
    bZoomInterp = true;
    TargetLength = OriginLength;
}

void UAPSpringArmComponent::ZoomConstant(float InOutLength, float ZoomTime)
{
    ZoomSpeed = abs(InOutLength)/ZoomTime;
    TargetLength = TargetArmLength + InOutLength;
    
    bZoomInterp = false;
    bZoomConstant = true;
}

void UAPSpringArmComponent::ZoomImmediate(float InOutLength)
{
    if(bZoomConstant || bZoomInterp) return;
    TargetArmLength = FMath::Clamp(TargetArmLength + InOutLength, MinimumSpringArmLength, MaximumSpringArmLength);
}

void UAPSpringArmComponent::ZoomInterpto(float InOutLength, float Speed)
{
    ZoomSpeed = Speed;
    TargetLength = TargetArmLength + InOutLength;

    bZoomConstant = false;
    bZoomInterp = true;
}
