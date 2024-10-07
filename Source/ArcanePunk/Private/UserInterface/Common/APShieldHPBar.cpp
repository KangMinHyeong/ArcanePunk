
#include "UserInterface/Common/APShieldHPBar.h"

#include "Components/ProgressBar.h"
#include "Character/APCharacterBase.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

void UAPShieldHPBar::NativeConstruct()
{
    Super::NativeConstruct();

}

void UAPShieldHPBar::CreateShieldHPBar(AActor* Owner)
{
    if(ShieldHPBar->GetPercent() <= KINDA_SMALL_NUMBER)
    {
        ShieldHPBar->SetPercent(1.0f);
        ShieldEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(ShieldEffect, Owner->GetRootComponent(), TEXT("ShieldEffect"), FVector::ZeroVector, FRotator::ZeroRotator, FVector(1.f,1.f,1.f), EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
        if(ShieldEffectComp.IsValid()) ShieldEffectComp->SetVariableFloat(TEXT("Size"), 1.0f / Owner->GetActorScale3D().X);
    }
}

void UAPShieldHPBar::BreakShieldHPBar()
{
    ShieldHPBar->SetPercent(0.0f);
    OnDestroyShield();
    if(ShieldEffectComp.IsValid()) ShieldEffectComp->DeactivateImmediate();
}

void UAPShieldHPBar::CheckShieldHP(AActor* Owner)
{
    auto OwnerCharacter = Cast<AAPCharacterBase>(Owner);
    if(!OwnerCharacter) return;
    
    auto ShieldHP = OwnerCharacter->GetPlayerStatus().StatusData.ShieldHP;
    if(ShieldHP <= KINDA_SMALL_NUMBER) {BreakShieldHPBar();}
    else {CreateShieldHPBar(OwnerCharacter);}
}
