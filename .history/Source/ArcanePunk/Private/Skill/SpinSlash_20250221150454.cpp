
#include "Skill/SpinSlash.h"

#include "Kismet/GameplayStatics.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Enemy/Enemy_CharacterBase.h"

ASpinSlash::ASpinSlash()
{
    SlashTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("SlashTrigger"));

    SetRootComponent(SlashTrigger);
}

void ASpinSlash::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);  
}

void ASpinSlash::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bCharging)
    {
        OnKeyDownCheck(DeltaTime);
    }
}

void ASpinSlash::DestroySKill()
{
    if(!OwnerCharacter.IsValid()) return;
    OwnerCharacter->GetAPHUD()->GetStatusWidget()->ChargeEnd(false);
    DeActivate(ESkillNumber::Skill_18);
    SlashEffectComp->Deactivate();
    Super::DestroySKill();
}

void ASpinSlash::OnKeyDownCheck(float DeltaTime)
{
    if(!OwnerCharacter.IsValid()) return;
    if(CheckSkillKey(SkillComp.Get()))
    {
        OwnerCharacter->GetAPHUD()->OnChargingGauge.Broadcast( CurrentTime, DestroyTime );
        CurrentTime = FMath::FInterpConstantTo(CurrentTime, DestroyTime, DeltaTime, ChargeSpeed);
    }
    else
    {
        DestroySKill();
    }
}

void ASpinSlash::OnSlashDamage()
{
    FHitResult HitResult;
    TArray<AActor*> Actors;
    SlashTrigger->GetOverlappingActors(Actors, AEnemy_CharacterBase::StaticClass());
    for(auto Enemy : Actors)
    {
        HitDelay(Enemy, OwnerCharacter->GetCurrentATK() * DamageCoefficient, HitNumbers, HitDelayTime, true);
    }
}

void ASpinSlash::SetSlashWidth(float Width)
{
    SlashWidth = SlashTrigger->GetScaledBoxExtent().X;
    SlashSize = Width / SlashWidth;
}

void ASpinSlash::SetSlashSize()
{
    float Width = SlashSize * SlashWidth;
    FVector BoxSize = FVector(Width, Width , Width);
    SlashTrigger->SetBoxExtent(BoxSize);

    SlashEffectComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SlashEffect, GetActorLocation(), GetActorRotation());
    SlashEffectComp->SetVariableVec3(TEXT("BoxSize"), BoxSize);
    SlashEffectComp->SetVariableFloat(TEXT("Time"), DestroyTime);
}

void ASpinSlash::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());  if(!OwnerCharacter.IsValid()) return;

    SetSlashSize();
    OnSlashDamage();
    GetWorldTimerManager().SetTimer(SlashTimerHandle, this, &ASpinSlash::OnSlashDamage, SlashAttackRate, true);
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ASpinSlash::DestroySKill, DestroyTime, false);  
}
