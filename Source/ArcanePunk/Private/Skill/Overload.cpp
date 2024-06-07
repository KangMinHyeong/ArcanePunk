
#include "Skill/Overload.h"

#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/SphereComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Components/Character/APSkillHubComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"

AOverload::AOverload()
{
    OverloadTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("OverloadTrigger"));
    OverloadRangeEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("OverloadRangeEffect"));

    SetRootComponent(OverloadTrigger);
    OverloadRangeEffect->SetupAttachment(OverloadTrigger);


}

void AOverload::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);  
    DamageCoefficient = 1.0f; 
}

void AOverload::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bCharging)
    {
        FireChecking(DeltaTime);
    }
}

void AOverload::DestroySKill()
{
    DeActivate_Ult();
    Super::DestroySKill();
}

void AOverload::SetRange(float Radius)
{
    OverloadRange = OverloadTrigger->GetScaledSphereRadius();
    RangeSize = Radius / OverloadRange;
}

void AOverload::FireChecking(float DeltaTime)
{
    if(!OwnerCharacter.IsValid()) return;
    if(OwnerCharacter->GetOnRSkill())
    {
        OwnerCharacter->GetAPHUD()->OnChargingGauge.Broadcast( DamageCoefficient - ChargeCurrent , ChargeMax );
        DamageCoefficient = FMath::FInterpConstantTo(DamageCoefficient, MaxDamageCoefficient, DeltaTime, ChargeSpeed);
    }
    else
    {
        bCharging = false;
        SetActorTickEnabled(false);
        auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
        if(OwnerAnim) OwnerAnim->StopUltSkill_11_Montage();
    }
}

void AOverload::ChargeEffectEnd()
{
    OwnerCharacter->GetAPHUD()->OnChargingEnd.Broadcast(false);
    OverloadRangeEffect->DeactivateImmediate();
    auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OverloadFireEffect, GetActorLocation(), GetActorRotation());
    NC->SetNiagaraVariableFloat(TEXT("Size"), RangeSize);
}

void AOverload::StartFire()
{
    FHitResult HitResult;
    TArray<AActor*> Actors;
    OverloadTrigger->GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
    for(auto Enemy : Actors)
    {
        float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Enemy, OwnerCharacter->GetCurrentATK() * DamageCoefficient, HitResult, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(Enemy, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
        // Effect 재생
    }
    DestroySKill();
}

void AOverload::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;

    OverloadTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    ChargeSpeed = (MaxDamageCoefficient - DamageCoefficient) / MaxTime;

    OverloadTrigger->SetSphereRadius(RangeSize * OverloadRange);
    OverloadRangeEffect->SetNiagaraVariableFloat(TEXT("Size"), RangeSize);
    OverloadRangeEffect->Activate();

    auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
    if(OwnerAnim) OwnerAnim->OnUltChargeEnd.AddUObject(this, &AOverload::ChargeEffectEnd);

    ChargeMax = MaxDamageCoefficient - DamageCoefficient;
    ChargeCurrent = DamageCoefficient;
}
