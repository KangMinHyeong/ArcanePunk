
#include "Components/Character/APSpringArmComponent.h"

void UAPSpringArmComponent::BeginPlay()
{
    Super::BeginPlay();
    OriginLength = TargetArmLength;
}

void UAPSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(bZoom)
    {
        TargetArmLength = FMath::FInterpConstantTo(TargetArmLength, TargetLength, DeltaTime, ZoomSpeed);
        TargetArmLength = FMath::Clamp(TargetArmLength, MinimumSpringArmLength, MaximumSpringArmLength);
        
        if(abs(TargetLength - TargetArmLength) <= KINDA_SMALL_NUMBER) bZoom = false;
    }
}

void UAPSpringArmComponent::Restore()
{
    bZoom = true;
    TargetLength = OriginLength;
}

void UAPSpringArmComponent::Zoom(float InOutLength, float ZoomTime)
{
    ZoomSpeed = abs(InOutLength)/ZoomTime;
    TargetLength = TargetArmLength + InOutLength;
    bZoom = true;
}

void UAPSpringArmComponent::ZoomImmediate(float InOutLength)
{
    if(bZoom) return;
    TargetArmLength = FMath::Clamp(TargetArmLength + InOutLength, MinimumSpringArmLength, MaximumSpringArmLength);
}

