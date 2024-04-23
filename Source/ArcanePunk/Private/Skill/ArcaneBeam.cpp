
#include "Skill/ArcaneBeam.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/BoxComponent.h" 
#include "Components/CapsuleComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"

AArcaneBeam::AArcaneBeam()
{    
    BeamBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BeamBox"));

    SetRootComponent(BeamBox);

    SkillCategory = ESkillCategory::Laser;
}

void AArcaneBeam::BeginPlay()
{
    Super::BeginPlay();
    IncreasingSpeed = InitIncreasingSpeed;
}

void AArcaneBeam::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bCharging)
    {
        OnChargindCheck(DeltaTime);
    }
}

void AArcaneBeam::DestroySKill()
{
    GetWorldTimerManager().ClearTimer(BeamHitLoopTimerHandle);
    if(BeamComp.IsValid()) {BeamComp->DestroyComponent();}
    Super::DestroySKill();
}

void AArcaneBeam::OnChargindCheck(float DeltaTime)
{
    if(!OwnerCharacter.IsValid()) return;
    if(CheckSkillKey(Skill_5.Get()))
    {
        OwnerCharacter->GetAPHUD()->OnChargingGauge.Broadcast( DamageCoefficient - ChargeCurrent , ChargeMax );
        DamageCoefficient = FMath::FInterpConstantTo(DamageCoefficient, MaxDamageCoefficient, DeltaTime, IncreasingSpeed);
    }
    else
    {
        bCharging = false;
        SetActorTickEnabled(false);
        auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
        if(OwnerAnim) OwnerAnim->StopSkill_5_Montage();
    }
}

void AArcaneBeam::SetBeamEffect()
{ 
     OwnerCharacter->GetAPHUD()->OnChargingEnd.Broadcast(false);
    if(ChargeEffectComp.IsValid()) ChargeEffectComp->DestroyComponent();
    if(ChargeEnhanceEffectComp.IsValid()) ChargeEnhanceEffectComp->DestroyComponent();

    if(!OwnerCharacter.IsValid()) return;
    float Size =  GetActorScale3D().Y / DefaultSize * ( 1.0f / OwnerCharacter->GetMesh()->GetComponentScale().Y);
    
    BeamComp = UNiagaraFunctionLibrary::SpawnSystemAttached(BeamEffect, OwnerCharacter->GetLeftBeamPoint(), TEXT("ArcaneBeam"), FVector(0,0,0), FRotator::ZeroRotator, Size * BeamScale,EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
    if(!BeamComp.IsValid()) return; 
    BeamStart = OwnerCharacter->GetLeftBeamPoint()->GetComponentLocation();
    BeamEnd = FVector(OwnerCharacter->GetCapsuleComponent()->GetComponentLocation().X,OwnerCharacter->GetCapsuleComponent()->GetComponentLocation().Y,BeamStart.Z) + OwnerCharacter->GetActorForwardVector() * Distance * Size;
    BeamComp->SetNiagaraVariableVec3(TEXT("Beam End"),  BeamEnd);

    SetBeamAttack();
}

void AArcaneBeam::SetBeamAttack()
{
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->GetAttackComponent()->MultiAttack(BeamStart, BeamEnd, BeamRadius, DamageCoefficient, 1, bStun, StateTime);
    GetWorldTimerManager().SetTimer(BeamHitLoopTimerHandle, this, &AArcaneBeam::SetBeamAttack, BeamHitLoopTime, false);
}

void AArcaneBeam::OnCharging()
{
    Super::OnCharging();    
    ChargeEffectComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ChargeEffect, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());
}

void AArcaneBeam::OnChargingEnhance()
{
    if(!OwnerCharacter.IsValid()) return;
    IncreasingSpeed = InitIncreasingSpeed * 3.0;
	ChargeEnhanceEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(ChargeEnhanceEffect, OwnerCharacter->GetMesh(), TEXT("BeamPoint"), OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
}

void AArcaneBeam::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());  if(!OwnerCharacter.IsValid()) return;

    Skill_5 = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(ESkillNumber::Skill_5);

    ChargeMax = MaxDamageCoefficient - DamageCoefficient;
    ChargeCurrent = DamageCoefficient;
}
