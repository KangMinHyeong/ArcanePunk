
#include "Skill/BlackHole.h"

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

ABlackHole::ABlackHole()
{
    GravityTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("GravityTrigger"));
    DamageTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("DamageTrigger"));
    BlackHoleEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BlackHoleEffect"));

    SetRootComponent(GravityTrigger);
    DamageTrigger->SetupAttachment(GravityTrigger);
    BlackHoleEffect->SetupAttachment(GravityTrigger);
}

void ABlackHole::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void ABlackHole::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(!OwnerCharacter.IsValid()) return;
    TArray<AActor*> DragegedActors;
    GravityTrigger->GetOverlappingActors(DragegedActors, AEnemy_CharacterBase::StaticClass());
    for(auto Actor : DragegedActors)
    {
        auto Enemy = Cast<AEnemy_CharacterBase>(Actor); if(!Enemy) continue; 
        if(Enemy->GetDefaultHP() <= KINDA_SMALL_NUMBER ) continue;
        FVector Current = Enemy->GetActorLocation();
        Current = FMath::VInterpConstantTo(Current, GetActorLocation(), DeltaTime, GravitySpeed);
        Enemy->SetActorLocation(Current); 
    } 
}

void ABlackHole::DestroySKill()
{
    DeActivate_Ult();
    Super::DestroySKill();
}

void ABlackHole::SetHoleRadius(float Radius, float Radius_2)
{
    OutSize =  Radius;
    InSize =  Radius_2; 
}

void ABlackHole::SetBlackHoleSize()
{
    BlackHoleEffect->SetVariableFloat(TEXT("OutSize"),  OutSize / GravityTrigger->GetScaledSphereRadius());
    BlackHoleEffect->SetVariableFloat(TEXT("Size"),  InSize / DamageTrigger->GetScaledSphereRadius());

    GravityTrigger->SetSphereRadius(OutSize); DamageTrigger->SetSphereRadius(InSize);
}

void ABlackHole::OnGravity()
{
    GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ABlackHole::OnDamage, LifeTime-DamageRate, true);
    GetWorldTimerManager().SetTimer(SlowTimerHandle, this, &ABlackHole::OnSlow, SlowRate, true);
    GravityTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    DamageTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    SetActorTickEnabled(true);
}

void ABlackHole::OnDamage()
{
    if(!OwnerCharacter.IsValid()) return;
    TArray<AActor*> DamagedActors;
    DamageTrigger->GetOverlappingActors(DamagedActors, AEnemy_CharacterBase::StaticClass());
    for(auto Actor : DamagedActors)
    {
        FHitResult HitResult;
        float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(Actor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, HitResult, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(Actor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
    } 
}

void ABlackHole::OnSlow()
{
    if(!OwnerCharacter.IsValid()) return;
    TArray<AActor*> SlowActors;
    GravityTrigger->GetOverlappingActors(SlowActors, AEnemy_CharacterBase::StaticClass());
    for(auto Actor : SlowActors)
    {
        UpdateSlowPercent(Actor->GetActorLocation());
        auto Enemy = Cast<AEnemy_CharacterBase>(Actor); 
        HitPointComp->SetCrowdControl(Actor, ECharacterState::Slow, SlowRate-0.001f);
        // Enemy->GetCrowdControlComp()-SlowState(int32 SlowPercent, float SlowTime)
    } 
}

void ABlackHole::UpdateSlowPercent(const FVector & EnemyLocation)
{
    FVector Current = GetActorLocation() - EnemyLocation; Current.Z = 0.0f;
    float size = (GravityTrigger->GetScaledSphereRadius() - Current.Size()) / GravityTrigger->GetScaledSphereRadius(); size = FMath::Max(0.0f, size);
    HitPointComp->SetSlowPercent(SlowPercent * size);
}

void ABlackHole::SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;
    
    BlackHoleEffect->SetVariableFloat(TEXT("CastingTime"),  CastingTime);
    BlackHoleEffect->SetVariableFloat(TEXT("Lifetime"),  LifeTime);
    BlackHoleEffect->Activate(true);

    SetBlackHoleSize();

    GetWorldTimerManager().SetTimer(TriggerTimerHandle, this, &ABlackHole::OnGravity, CastingTime, false);
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ABlackHole::DestroySKill, CastingTime + LifeTime, false);
}
