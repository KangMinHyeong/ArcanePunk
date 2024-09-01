
#include "AnimInstance/APTrapAnimInstance.h"

#include "GameElements/Trap/APTrapBase.h"

UAPTrapAnimInstance::UAPTrapAnimInstance()
{
}

void UAPTrapAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
}

float UAPTrapAnimInstance::PlayTrapOperation_Montage()
{
    UE_LOG(LogTemp, Display, TEXT("PlayTrapOperation_Montage"));
    return Montage_Play(TrapOperation_Montage);
}

void UAPTrapAnimInstance::AnimNotify_DamageTrigger()
{
    auto Trap = Cast<AAPTrapBase>(GetOwningActor());
    if(Trap) Trap->OnDamageTrigger();
}
