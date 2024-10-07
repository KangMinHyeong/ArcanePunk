
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
    return Montage_Play(TrapOperation_Montage);
}

float UAPTrapAnimInstance::PlayReload_Montage()
{
    return Montage_Play(TrapReload_Montage);
}

void UAPTrapAnimInstance::AnimNotify_DamageTrigger()
{
    auto Trap = Cast<AAPTrapBase>(GetOwningActor());
    if(Trap) Trap->OnDamageTrigger();
}

void UAPTrapAnimInstance::AnimNotify_SpawnTrapOperationSound()
{
    auto Trap = Cast<AAPTrapBase>(GetOwningActor());
    if(Trap) Trap->SpawnTrapOperationSound();
}
