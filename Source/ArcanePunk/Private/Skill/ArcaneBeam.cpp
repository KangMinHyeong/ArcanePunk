
#include "Skill/ArcaneBeam.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/BoxComponent.h"

AArcaneBeam::AArcaneBeam()
{    
    BeamBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BeamBox"));

    SetRootComponent(BeamBox);

    SkillCategory = ESkillCategory::Laser;
}

void AArcaneBeam::BeginPlay()
{
    Super::BeginPlay();


}

void AArcaneBeam::DestroySKill()
{
    Super::DestroySKill();
    GetWorldTimerManager().ClearTimer(BeamHitLoopTimerHandle);
    if(BeamComp) {BeamComp->DestroyComponent();}
}

void AArcaneBeam::SetBeamEffect()
{ 
    float Size =  GetActorScale3D().Y / DefaultSize;
    auto Character = Cast<AArcanePunkCharacter>(GetOwner()); if(!Character) return;
    BeamComp = UNiagaraFunctionLibrary::SpawnSystemAttached(BeamEffect, Character->GetLeftBeamPoint(), TEXT("ArcaneBeam"), FVector(0,0,0), FRotator::ZeroRotator, Size * BeamScale,EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
    if(!BeamComp) return; 
    BeamStart = Character->GetLeftBeamPoint()->GetComponentLocation();
    BeamEnd = FVector(Character->GetMesh()->GetComponentLocation().X,Character->GetMesh()->GetComponentLocation().Y,BeamStart.Z) + Character->GetActorForwardVector() * Distance * Size;
    BeamComp->SetNiagaraVariableVec3(TEXT("Beam End"),  BeamEnd);
    BeamComp->SetNiagaraVariableLinearColor(TEXT("Color"),  EffectColor);
    SetBeamAttack();
}

void AArcaneBeam::SetBeamAttack()
{
    auto Character = Cast<AArcanePunkCharacter>(GetOwner()); if(!Character) return;
    Character->GetAttackComponent()->MultiAttack(BeamStart, BeamEnd, BeamRadius, DamageCoefficient, 1, bStun, StateTime);
    GetWorldTimerManager().SetTimer(BeamHitLoopTimerHandle, this, &AArcaneBeam::SetBeamAttack, BeamHitLoopTime, false);
}

void AArcaneBeam::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
    Super::SetSkill(SkillType, SkillAbility);
    
}
