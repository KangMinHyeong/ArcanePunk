
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
#include "Character/SkillRange/APSkillRange.h"
#include "Enemy/Enemy_CharacterBase.h"

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
    BeamTime = OriginTime;
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
    if(CheckSkillKey(SkillComp.Get()))
    {
        OwnerCharacter->GetAPHUD()->OnChargingGauge.Broadcast( DamageCoefficient - ChargeCurrent , ChargeMax );
        DamageCoefficient = FMath::FInterpConstantTo(DamageCoefficient, MaxDamageCoefficient, DeltaTime, IncreasingSpeed);
    }
    else
    {        
        SetSkillComp();
    }
}

void AArcaneBeam::SetSkillComp()
{
    if(!SkillComp.IsValid()) return;
    SkillComp->Remove_Skill();
}

void AArcaneBeam::SetBeamEffect()
{ 
    GetWorldTimerManager().SetTimer(BeamTimerHandle, this, &AArcaneBeam::FireEnd, BeamTime, false);
    OwnerCharacter->GetAPHUD()->OnChargingEnd.Broadcast(false);
    if(ChargeEffectComp.IsValid()) ChargeEffectComp->DestroyComponent();
    if(ChargeEnhanceEffectComp.IsValid()) ChargeEnhanceEffectComp->DestroyComponent();

    if(!OwnerCharacter.IsValid()) return;
    float Size =  GetActorScale3D().Y / DefaultSize * ( 1.0f / OwnerCharacter->GetMesh()->GetComponentScale().Y);
    
    BeamComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BeamEffect, OwnerCharacter->GetLeftBeamPoint()->GetComponentLocation(), OwnerCharacter->GetActorRotation());
    // UNiagaraFunctionLibrary::SpawnSystemAttached(BeamEffect, OwnerCharacter->GetLeftBeamPoint(), TEXT("ArcaneBeam"), FVector(0,0,0), FRotator::ZeroRotator, Size * BeamScale,EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true);
    if(!BeamComp.IsValid()) return; 
    BeamStart = OwnerCharacter->GetLeftBeamPoint()->GetComponentLocation();
    BeamEnd = FVector(OwnerCharacter->GetCapsuleComponent()->GetComponentLocation().X,OwnerCharacter->GetCapsuleComponent()->GetComponentLocation().Y,BeamStart.Z) + OwnerCharacter->GetActorForwardVector() * Distance * Size;
    
    float Width = BeamRadius / InitRadius;
    float Length = Distance / InitDist;
    // BeamComp->SetNiagaraVariableVec3(TEXT("Beam End"),  BeamEnd);
    BeamComp->SetNiagaraVariableFloat(TEXT("AddTime"),  BeamTime - OriginTime);
    BeamComp->SetNiagaraVariableFloat(TEXT("Width"),  Width);
    BeamComp->SetNiagaraVariableFloat(TEXT("Length"),  Length);
    BeamComp->SetNiagaraVariableVec2(TEXT("Size2D"),  FVector2D(Width, Length));
    BeamComp->SetNiagaraVariableVec3(TEXT("Size3D"),  FVector(Width, Length, 1.0f));
    BeamComp->SetNiagaraVariableVec3(TEXT("AddVector"),  FVector((Distance - InitDist) * 0.5f, 0.0f, 0.0f));

    SetBeamAttack();
}

void AArcaneBeam::SetBeamAttack()
{
    if(!OwnerCharacter.IsValid()) return;
    auto Actors = OwnerCharacter->GetAttackComponent()->MultiAttack_Return(BeamStart, BeamEnd, BeamRadius, DamageCoefficient, 1, InstantDeathPercent, bStun, StateTime);
    GetWorldTimerManager().SetTimer(BeamHitLoopTimerHandle, this, &AArcaneBeam::SetBeamAttack, BeamHitLoopTime, false);
    
    if(AddCool)
    {
        if(KillsNum < 3)
        {
            for(auto Actor : Actors)
            {
                auto Enemy = Cast<AEnemy_CharacterBase>(Actor); if(!Enemy) continue;
                if(Enemy->IsDead()) {KillsNum++;}
            }
        }

        if(KillsNum >= 3)
        {
            UE_LOG(LogTemp, Display, TEXT("Your message"));
            SkillComp->AddCoolTime(AddCoolTime);
        }
    }
}

void AArcaneBeam::FireEnd()
{
    auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
    if(OwnerAnim) OwnerAnim->StopSkill_5_Fire_Montage();
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

void AArcaneBeam::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());  if(!OwnerCharacter.IsValid()) return;
    
    for(auto It : SkillAbilityNestingData.SilverAbilityNestingNum)
    {
        CheckSilverEnhance(It.Key, It.Value);
    }
    for(auto It : SkillAbilityNestingData.GoldAbilityNestingNum)
    {
        CheckGoldEnhance(It.Key, It.Value);
    }
    for(auto It : SkillAbilityNestingData.PlatinumAbilityNestingNum)
    {
        CheckPlatinumEnhance(It.Key, It.Value);
    }

    ChargeMax = MaxDamageCoefficient - DamageCoefficient;
    ChargeCurrent = DamageCoefficient;
}

void AArcaneBeam::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	Super::CheckSilverEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient,AbilityData->Coefficient_X, NestingNum);
        break;
    }
}

void AArcaneBeam::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckGoldEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient, AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // 지속시간
        SkillAbilityComponent->Coefficient_AddMultiple(BeamTime, AbilityData->Coefficient_X, NestingNum);
        break;
    }
}

void AArcaneBeam::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckPlatinumEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient,AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // 3 Kills 
        AddCool = true;
        SkillAbilityComponent->Coefficient_Add(AddCoolTime,AbilityData->Coefficient_X, NestingNum);
        break;
    }
}
