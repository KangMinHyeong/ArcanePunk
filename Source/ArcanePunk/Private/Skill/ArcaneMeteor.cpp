
#include "Skill/ArcaneMeteor.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Components/Common/APCrowdControlComponent.h"

AArcaneMeteor::AArcaneMeteor()
{
    DamageTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("DamageTrigger"));
    DamageEnhanceTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("DamageEnhanceTrigger"));
    MeteorEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MeteorEffect"));

    SetRootComponent(DamageTrigger);
    DamageEnhanceTrigger->SetupAttachment(DamageTrigger);
    MeteorEffect->SetupAttachment(DamageTrigger);
}

void AArcaneMeteor::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AArcaneMeteor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AArcaneMeteor::DestroySKill()
{
    Super::DestroySKill();
}

void AArcaneMeteor::SetRadius(float Radius_1, float Radius_2)
{
    TriggerRadius_1 = Radius_1; TriggerRadius_2 = Radius_2;
}

void AArcaneMeteor::OnMeteorDrop()
{
    GetWorldTimerManager().ClearTimer(MeteorTimerHandle);
    DamageTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    DamageEnhanceTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneMeteor::DestroySKill , DestroyTime+ 1.0f, false);
    
    TArray<AActor*> EnhanceActors; FHitResult HitResult;
    DamageEnhanceTrigger->GetOverlappingActors(EnhanceActors, AActor::StaticClass());
    for (AActor* Actor : EnhanceActors)
    {           

        float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Actor, OwnerCharacter->GetCurrentATK() * DamageCoefficient * EnhanceDamageCoefficient, HitResult, true);
	    OwnerCharacter->GetAttackComponent()->DrainCheck(Actor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());       
    } 

    TArray<AActor*> NormalActors;
    DamageTrigger->GetOverlappingActors(NormalActors, AActor::StaticClass());
    for (AActor* Actor : NormalActors)
    {
        if(EnhanceActors.Contains(Actor)) continue;       
      
        float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Actor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, HitResult, true);
	    OwnerCharacter->GetAttackComponent()->DrainCheck(Actor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());           
    } 

    GetWorldTimerManager().SetTimer(MeteorTimerHandle, this, &AArcaneMeteor::BurnDamage , BurnRate, true);
}

void AArcaneMeteor::BurnDamage()
{
    if(BurnNumber == 0) {GetWorldTimerManager().ClearTimer(MeteorTimerHandle); return;}
    TArray<AActor*> EnhanceActors; FHitResult HitResult;
    DamageEnhanceTrigger->GetOverlappingActors(EnhanceActors, AEnemy_CharacterBase::StaticClass());
    for (AActor* Actor : EnhanceActors)
    {           
        auto Enemy = Cast<AEnemy_CharacterBase>(Actor); 
        if(Enemy) OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Enemy, DotDamage, HitResult, false); 
    } 
    BurnNumber--;
}

void AArcaneMeteor::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;
    
    float Multiple = TriggerRadius_2 / DamageEnhanceTrigger->GetScaledSphereRadius();
    DamageTrigger->SetSphereRadius(TriggerRadius_1); DamageEnhanceTrigger->SetSphereRadius(TriggerRadius_2);

    MeteorEffect->SetVariableFloat(TEXT("Size"),  MeteorSize * Multiple);
    MeteorEffect->SetVariableFloat(TEXT("Lifetime"),  DestroyTime);
    // Effect 시간 조절
    GetWorldTimerManager().SetTimer(MeteorTimerHandle, this, &AArcaneMeteor::OnMeteorDrop, CastingDelay, false);
}