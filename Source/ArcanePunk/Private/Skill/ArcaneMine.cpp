
#include "Skill/ArcaneMine.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Enemy/Enemy_CharacterBase.h"

AArcaneMine::AArcaneMine()
{
	MineTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("MineTrigger"));
	MineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MineMesh"));

    SetRootComponent(MineTrigger);
    MineMesh->SetupAttachment(MineTrigger);
	
    MineMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MineMoveComp"));
	MineMoveComp->MaxSpeed = 0.0f;
	MineMoveComp->InitialSpeed = 0.0f;

    SkillCategory = ESkillCategory::Projecitle;
}

void AArcaneMine::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
    // BintHit();
}

void AArcaneMine::Tick(float DeltaTime)
{
    CurrentLocation = GetActorLocation();
    CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MineSpeed);
    SetActorLocation(CurrentLocation);
    // FVector Target = MineMoveComp->HomingTargetComponent->GetComponentLocation();
    if(abs(TargetLocation.X - GetActorLocation().X) < 0.01f && TargetLocation.Y - GetActorLocation().Y < 0.01f)
    {
        bExploaionEnable = true;
        GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneMine::Explosion, DestroyTime, false);
        SetActorTickEnabled(false);
        MineMoveComp->SetVelocityInLocalSpace(FVector(0.0f, 0.0f, 0.0f));
    }
}

void AArcaneMine::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
        Explosion();
	}
}

void AArcaneMine::Explosion()
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());  if(!OwnerCharacter.IsValid()) return;
    float Size =  GetActorScale3D().Y / DefaultSize;

    TArray<AActor*> Actors;
    if(!bStun){Actors = OwnerCharacter->GetAttackComponent()->MultiAttack_KnockBack_Return(GetActorLocation(), GetActorLocation() + OwnerCharacter->GetActorUpVector() * 25.0f, ExplosionRadius, ExplosionRadius*2.0f, DamageCoefficient, HitNumbers, InstantDeathPercent, StateTime, true);}
    else {Actors = OwnerCharacter->GetAttackComponent()->MultiAttack_Return(GetActorLocation(), GetActorLocation() + OwnerCharacter->GetActorUpVector() * 25.0f, ExplosionRadius, DamageCoefficient, HitNumbers, InstantDeathPercent, true, StateTime);}

    for(auto Actor : Actors)
    {
        auto Enemy = Cast<AEnemy_CharacterBase>(Actor); if(!Enemy) continue;
        Enemy->GetCrowdControlComp()->BurnState(OwnerCharacter.Get(), DOT, DOT_Time, DOT_InRate);
        if(bSlow) Enemy->GetCrowdControlComp()->SlowState(SlowPercent, DOT_Time);
        if(bWeak) Enemy->GetCrowdControlComp()->WeakState(WeakCoefficient, DOT_Time);
        if(BuffType == EBuffType::Damage) {Enemy->AddDamageMultiple(DamageBuff, DOT_Time - 0.001f);}
    }
    // OwnerCharacter->GetAttackComponent()->MultiAttack_Burn(GetActorLocation(), GetActorLocation() + OwnerCharacter->GetActorUpVector() * 25.0f, ExplosionRadius, DOT, DOT_Time, DOT_InRate );
    // if(bSlow) OwnerCharacter->GetAttackComponent()->MultiAttack_Slow(GetActorLocation(), GetActorLocation() + OwnerCharacter->GetActorUpVector() * 25.0f, ExplosionRadius, SlowPercent, DOT_Time);

    // if(ExplosionEffect) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), ExplosionScale);
    if(ExplosionEffect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), ExplosionScale);
    DestroySKill();
}

void AArcaneMine::SetTarget(FVector TargetComponent)
{
    // MineMoveComp->HomingTargetComponent = TargetComponent;
    
    TargetLocation = TargetComponent;
}

void AArcaneMine::BintHit()
{
    MineTrigger->OnComponentHit.AddDynamic(this, &AArcaneMine::OnHitting);
}

void AArcaneMine::DestroySKill()
{
    DeActivate(ESkillNumber::Skill_9);
    Super::DestroySKill();
}

void AArcaneMine::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;

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

    SetActorTickEnabled(true);
}

void AArcaneMine::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	Super::CheckSilverEnhance(AbilityNum, NestingNum);

    float UpdateSize;
    switch (AbilityNum)
    {
        case 1: // 투사체 사거리 증가, SkillNumber9에서 이미 처리
        break;

        case 2: // 투사체 크기 증가
        UpdateSize = SkillAbilityComponent->SizeUp_Multiple(AbilityData->Coefficient_X, NestingNum);
        SetActorScale3D(GetActorScale3D()* UpdateSize);
        ExplosionRadius = ExplosionRadius * UpdateSize;
        break;

        case 3: // 넉백대신 기절
        bStun = true;
        break;

        case 4:
        SkillAbilityComponent->Coefficient_Multiple(DOT_Time, AbilityData->Coefficient_X, NestingNum);
        SkillAbilityComponent->Coefficient_Multiple(DOT, AbilityData->Coefficient_Y, NestingNum);
        break;
    }
}

void AArcaneMine::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckGoldEnhance(AbilityNum, NestingNum);

    DamageBuff = 0.0f;
    switch (AbilityNum)
    {
        case 1: // 스킬 데미지 증가
        SkillAbilityComponent->Coefficient_Add(DamageBuff, AbilityData->Coefficient_X, NestingNum);
        BuffType = EBuffType::Damage;
        break;

        case 2: // 피해 지속시간 증가
        SkillAbilityComponent->Coefficient_Add(DOT_Time, AbilityData->Coefficient_X, NestingNum);
        break;

        case 3: // 이동속도 감소
        bSlow = true;
        SkillAbilityComponent->Coefficient_Add(SlowPercent, AbilityData->Coefficient_X, NestingNum);
        SlowPercent = FMath::Min(90, SlowPercent);
        break;
        
        case 4: // 공격력 감소
        bWeak = true;
        SkillAbilityComponent->Coefficient_Add(WeakCoefficient, AbilityData->Coefficient_X, NestingNum);
        WeakCoefficient = FMath::Min(0.9, WeakCoefficient);
        break;
    }
}

void AArcaneMine::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckPlatinumEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 즉사
        InstantDeathPercent =  (AbilityData->Coefficient_X);
        break;

        case 2: // 충전횟수, 이미 처리
        break;
    }
}
