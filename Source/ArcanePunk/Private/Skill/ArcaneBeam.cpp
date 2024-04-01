
#include "Skill/ArcaneBeam.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/BoxComponent.h" 
#include "Components/CapsuleComponent.h"

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
    if(BeamComp.IsValid()) {BeamComp->DestroyComponent();}
}

void AArcaneBeam::SetBeamEffect(float Coefficient)
{ 
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
    float Size =  GetActorScale3D().Y / DefaultSize * ( 1.0f / OwnerCharacter->GetMesh()->GetComponentScale().Y);
    
    BeamComp = UNiagaraFunctionLibrary::SpawnSystemAttached(BeamEasdaffect, OwnerCharacter->GetLeftBeamPoint(), TEXT("ArcaneBeam"), FVector(0,0,0), FRotator::ZeroRotator, Size * BeamScale,EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
    if(!BeamComp.IsValid()) return; 
    BeamStart = OwnerCharacter->GetLeftBeamPoint()->GetComponentLocation();
    BeamEnd = FVector(OwnerCharacter->GetCapsuleComponent()->GetComponentLocation().X,OwnerCharacter->GetCapsuleComponent()->GetComponentLocation().Y,BeamStart.Z) + OwnerCharacter->GetActorForwardVector() * Distance * Size;
    BeamComp->SetNiagaraVariableVec3(TEXT("Beam End"),  BeamEnd);
    // BeamComp->SetNiagaraVariableLinearColor(TEXT("Color"),  EffectColor);

    DamageCoefficient = Coefficient;
    SetBeamAttack();
}

void AArcaneBeam::SetBeamAttack()
{
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->GetAttackComponent()->MultiAttack(BeamStart, BeamEnd, BeamRadius, DamageCoefficient, 1, bStun, StateTime);
    GetWorldTimerManager().SetTimer(BeamHitLoopTimerHandle, this, &AArcaneBeam::SetBeamAttack, BeamHitLoopTime, false);
}

void AArcaneBeam::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);
    
}
