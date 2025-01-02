
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

void UAPShieldHPBar::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(!bChange || !OwnerCharacter.IsValid()) return; 
    const auto PD = OwnerCharacter->GetPlayerStatus();
    
    CurrentOriginShieldHP = FMath::FInterpConstantTo(CurrentOriginShieldHP, PD.StatusData.ShieldHP, InDeltaTime, ShieldPercentSpeed);
    ShieldHPBar->SetPercent(CurrentOriginShieldHP / PD.StatusData.MaxHP);
    if(abs(CurrentOriginShieldHP - PD.StatusData.ShieldHP) <= KINDA_SMALL_NUMBER) bChange = false;    
}

void UAPShieldHPBar::CreateShieldHPBar()
{
    if(!OwnerCharacter.IsValid()) return; 

    bChange = false; 

    const auto PD = OwnerCharacter->GetPlayerStatus();
    ShieldHPBar->SetPercent(PD.StatusData.ShieldHP / PD.StatusData.MaxHP);
    ShieldEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(ShieldEffect, OwnerCharacter->GetRootComponent(), TEXT("ShieldEffect"), FVector::ZeroVector, FRotator::ZeroRotator, FVector(1.f,1.f,1.f), EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
    if(ShieldEffectComp.IsValid()) ShieldEffectComp->SetVariableFloat(TEXT("Size"), 1.0f / OwnerCharacter->GetActorScale3D().X);
}

void UAPShieldHPBar::BreakShieldHPBar()
{
    bChange = false; 

    ShieldHPBar->SetPercent(0.0f);
    OnDestroyShield();
    if(ShieldEffectComp.IsValid()) ShieldEffectComp->DeactivateImmediate();
}

void UAPShieldHPBar::CheckShieldHP(AActor* Owner, float OriginShieldHP)
{
    OwnerCharacter = Cast<AAPCharacterBase>(Owner);
    if(!OwnerCharacter.IsValid()) return;
    
    auto ShieldHP = OwnerCharacter->GetPlayerStatus().StatusData.ShieldHP;
    CurrentOriginShieldHP = OriginShieldHP;
    bChange = true;

    if(OriginShieldHP <= KINDA_SMALL_NUMBER && ShieldHP > KINDA_SMALL_NUMBER) {CreateShieldHPBar();}
    else if(OriginShieldHP > KINDA_SMALL_NUMBER && ShieldHP <= KINDA_SMALL_NUMBER) {BreakShieldHPBar();}
}
