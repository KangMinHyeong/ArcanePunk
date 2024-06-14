
#include "Skill/ArcaneCutter.h"

#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy/Enemy_CharacterBase.h"

AArcaneCutter::AArcaneCutter()
{
	CutterTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CutterTrigger"));

	SetRootComponent(CutterTrigger);

    CutterMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("CutterMovementComponent"));

    SkillCategory = ESkillCategory::Projecitle;
}

void AArcaneCutter::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);   
}

void AArcaneCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CutterSpeed = FMath::FInterpTo(CutterSpeed, 0.0f, DeltaTime, Drag);
	CutterMovementComponent->SetVelocityInLocalSpace(FVector(CutterSpeed, 0.0f, 0.0f));
	if(CutterSpeed <= 5.0f) {DestroySKill();}
}

void AArcaneCutter::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	OwnerCharacter = Cast<AArcanePunkCharacter>(MyOwner);
	if (OtherActor && OtherActor != this && OtherActor != MyOwner && OwnerCharacter.IsValid())
	{
		if(bStun) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Stun, StateTime);
		HitPointComp->DistinctHitPoint(Hit.Location, OtherActor);
		
		float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, Hit, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
		if(CutterEffectComp.IsValid()) CutterEffectComp->Deactivate();
	}
    DestroySKill();
}


void AArcaneCutter::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	
    if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		if(bStun) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Stun, StateTime);
		HitPointComp->DistinctHitPoint(OtherActor->GetActorLocation(), OtherActor);

        float DamageApplied = OwnerCharacter->GetAttackComponent()->ApplyDamageToActor(OtherActor, OwnerCharacter->GetCurrentATK() * DamageCoefficient, SweepResult, true);
		OwnerCharacter->GetAttackComponent()->DrainCheck(OtherActor, DamageApplied, OwnerCharacter->GetAttackComponent()->GetSkillDrainCoefficient());
	}

    PenetrateCount--;
    if(PenetrateCount == 0) {if(CutterEffectComp.IsValid()) CutterEffectComp->Deactivate(); DestroySKill();}
}

float AArcaneCutter::GetCutterSpeed() const
{
    return CutterSpeed;
}

void AArcaneCutter::SetCutterWide(float Wide)
{
    FVector Origin = CutterTrigger->GetScaledBoxExtent();
    WidthScale.X = 1.0f + (Wide /( 4.0f * Origin.Y)) ;
    CutterTrigger->SetBoxExtent(FVector(Origin.X, Wide, Origin.Z));
}
// void AArcaneCutter::SetDeadTime(float DeadTime)
// {
// 	DestroyTime = DeadTime;
// }

float AArcaneCutter::GetTriggerWide() const
{
    return CutterTrigger->GetScaledBoxExtent().X;
}

void AArcaneCutter::DestroySKill()
{
	Super::DestroySKill();

    if(bExplosion) Explosion();
}

void AArcaneCutter::Explosion()
{
    OwnerCharacter->GetAttackComponent()->MultiAttack(GetActorLocation(), GetActorLocation(), ExplosionRadius, ExplosionCoefficient, 1, false, StateTime);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());
}

void AArcaneCutter::BintHit()
{
    // CutterTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    if(bPenetrate)
    {
        CutterTrigger->OnComponentBeginOverlap.AddDynamic(this, &AArcaneCutter::OnOverlap);
        CutterTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    }
    else
    {
        CutterTrigger->OnComponentHit.AddDynamic(this, &AArcaneCutter::OnHitting);
        CutterTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
    }
    CutterTrigger->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);
    CutterTrigger->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block); 
    CutterTrigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Block); 
    CutterTrigger->SetCollisionResponseToChannel(ECC_Vehicle, ECollisionResponse::ECR_Block); 
}

void AArcaneCutter::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	CutterMovementComponent->SetVelocityInLocalSpace(FVector(CutterSpeed, 0.0f, 0.0f));

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
    
    CutterSpeed = CutterDist * Drag;
	CutterEffectComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CutterEffect, OwnerCharacter->GetMesh()->GetComponentLocation(), OwnerCharacter->GetActorRotation(), FVector(1.0f, 1.0f, 1.0f), true, false);
	if(CutterEffectComp.IsValid())
	{
        CutterEffectComp->DeactivateImmediate();
		CutterEffectComp->SetVariableFloat(TEXT("Speed"),  CutterSpeed);
		CutterEffectComp->SetVariableFloat(TEXT("Drag"),  Drag);
        CutterEffectComp->SetVariableVec2(TEXT("Size"), FVector2D(WidthScale.X,1.0f));
        CutterEffectComp->Activate();
	}
	
	BintHit();
	SetActorTickEnabled(true);
}


void AArcaneCutter::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	Super::CheckSilverEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient,AbilityData->Coefficient_X, NestingNum);
        break;

		case 3: // 속도 증가
		SkillAbilityComponent->Coefficient_AddMultiple(Drag, AbilityData->Coefficient_X, NestingNum);
        break;
    }
}

void AArcaneCutter::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckGoldEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient,AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // 관통
        SkillAbilityComponent->Coefficient_Add(PenetrateCount,AbilityData->Coefficient_X, NestingNum);
        PenetrateType = EPenetrateType::Enemy;
        bPenetrate = true;
        break;
    }
}

void AArcaneCutter::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckPlatinumEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // Damage Up
        SkillAbilityComponent->Coefficient_Add(DamageCoefficient,AbilityData->Coefficient_X, NestingNum);
        break;
        
        case 2: // Bang
        bExplosion = true;
        SkillAbilityComponent->Coefficient_Add(ExplosionCoefficient, AbilityData->Coefficient_X, NestingNum);
        break;
    }
}

