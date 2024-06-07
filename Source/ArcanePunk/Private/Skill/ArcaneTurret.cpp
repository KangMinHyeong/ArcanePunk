
#include "Skill/ArcaneTurret.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DamageText/DamageText.h"
#include "Particles/ParticleSystem.h"
#include "Skill/Sub/TurretAmmo.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Skill/Sub/TurretBeam.h"

AArcaneTurret::AArcaneTurret()
{   
    TurretCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TurretCapsule"));
    TurretRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretRoot"));
    RotateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateMesh"));
    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
     
    SetRootComponent(TurretCapsule);
    TurretRoot->SetupAttachment(TurretCapsule);
    RotateMesh->SetupAttachment(TurretRoot);
    ProjectileSpawnPoint->SetupAttachment(RotateMesh);

    SkillCategory = ESkillCategory::Spawn_Static;
}

void AArcaneTurret::BeginPlay()
{
    Super::BeginPlay();
    SetActorLocation(GetActorLocation() + FVector(0.0f,0.0f,TurretCapsule->GetScaledCapsuleHalfHeight()));
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);    
}

void AArcaneTurret::DestroySKill()
{
	Super::DestroySKill();
    if(DeadEffect) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeadEffect, GetActorLocation(), GetActorRotation(), DeadEffectSize);
    if(bDeadStun && OwnerCharacter.IsValid()) OwnerCharacter->GetAttackComponent()->MultiAttack_OnlyCC(GetActorLocation(), GetActorLocation() + GetActorUpVector() * 25.0f, DeadStunRadius, ECharacterState::Stun, StateTime);
}

float AArcaneTurret::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    if(bGodMode) return DamageApplied;
    if(Cast<AArcanePunkPlayerController>(EventInstigator)) return DamageApplied;

	DamageApplied = FMath::Min(HP, DamageApplied);
	
	HP = HP - DamageApplied;
	UE_LOG(LogTemp, Display, TEXT("Turret HP : %f"), HP);

    SpawnDamageText(EventInstigator, DamageAmount);

    if(HP <= 0.001f)
	{
	 	DestroySKill();
	}

    return DamageApplied;
}

void AArcaneTurret::Fire()
{
    if(!Enemy.IsValid()) return;

	auto ProjectileActor = GetWorld()->SpawnActor<ATurretAmmo>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	ProjectileActor->SetOwner(this);
    SpawnAmmoNum--;

    if(bEnhance)
    {
        EnhanceCount++;
        if(EnhanceCount == 3) {EnhanceCount = 0; GetWorldTimerManager().SetTimer(EnhanceTimerHandle, this, &AArcaneTurret::EnhanceFire, EnhanceTerm, false);}
    }
    
    if(SpawnAmmoNum == 0) {SpawnAmmoNum = OriginAmmoNum; GetWorldTimerManager().ClearTimer(SpawnLoopTimerHandle);}
    else {GetWorldTimerManager().SetTimer(SpawnLoopTimerHandle, this, &AArcaneTurret::Fire, SpawnLoopRate, false);}
}

void AArcaneTurret::EnhanceFire()
{
    GetWorldTimerManager().ClearTimer(EnhanceTimerHandle);
    auto BeamActor = GetWorld()->SpawnActor<ATurretBeam>(BeamClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	BeamActor->SetOwner(this);
}

void AArcaneTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Enemy.IsValid())
    {
        AutoAttack();
    }
    else
    {
        Enemy = FindEnemy();
    }
}

void AArcaneTurret::AutoAttack()
{
    FVector ToTarget = Enemy->GetActorLocation() - RotateMesh->GetComponentLocation();
	FRotator TargetRotation = FRotator(0, ToTarget.Rotation().Yaw, 0);

	RotateMesh->SetWorldRotation(FMath::RInterpTo(RotateMesh->GetComponentRotation(), TargetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), RotateSpeed));
}

AActor* AArcaneTurret::FindEnemy()
{
    TArray<AActor*> Actors; float Dist = AttackDistance;
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("Enemy"), Actors);
	AActor* Target = UGameplayStatics::FindNearestActor(GetActorLocation(), Actors, Dist);
	if(Target)
	{
		if(Dist <= AttackDistance)
		{
			return Target;
		}
	}
    return nullptr;
}

void AArcaneTurret::SpawnDamageText(AController *EventInstigator, float Damage)
{
    ADamageText* DamageText = GetWorld()->SpawnActor<ADamageText>(DamageTextClass, GetActorLocation(), FRotator(0.0f, 180.0f, 0.0f)); if(!DamageText) return;
    DamageText->SetOwner(this);

    OwnerCharacter = Cast<AArcanePunkCharacter>(EventInstigator->GetPawn()); 
    auto InstigatorEnemy =  Cast<AEnemy_CharacterBase>(Enemy);

    if(OwnerCharacter.IsValid()) DamageText->SetDamageText(Damage, OwnerCharacter->IsCriticalAttack());
    else if(InstigatorEnemy) DamageText->SetDamageText(Damage, InstigatorEnemy->IsCriticalAttack());
	
}

void AArcaneTurret::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent)
{
    Super::SetSkill(SkillAbilityNestingData, SkillComponent);
    if(!OwnerCharacter.IsValid()) return;
    
    CurrentDamage = OwnerCharacter->GetCurrentATK() * DamageCoefficient;
    
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

    OriginAmmoNum = SpawnAmmoNum;
    Enemy = FindEnemy();
    SetActorTickEnabled(true);
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AArcaneTurret::Fire, FireRate, true);
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneTurret::DestroySKill, DestroyTime, false);
}

void AArcaneTurret::CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum)
{
	Super::CheckSilverEnhance(AbilityNum, NestingNum);
    
    float Max;
    switch (AbilityNum)
    {
        case 1: // 지속시간 증가
        SkillAbilityComponent->Coefficient_Add(DestroyTime, AbilityData->Coefficient_X, NestingNum);
        break;

        case 2: // 공격속도 증가
        Max = FireRate * 0.1f;
        SkillAbilityComponent->Coefficient_SubtractMultiple(FireRate, AbilityData->Coefficient_X, NestingNum);
        FireRate = FMath::Max(Max, FireRate);
        break;

        case 3: // 공격력 증가
        Max = CurrentDamage * 1.9f;
        SkillAbilityComponent->Coefficient_AddMultiple(CurrentDamage, AbilityData->Coefficient_X, NestingNum);
        CurrentDamage = FMath::Min(Max, CurrentDamage);
        break;  
    }
}

void AArcaneTurret::CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum)
{
     Super::CheckGoldEnhance(AbilityNum, NestingNum);
    // float Max;

    switch (AbilityNum)
    {
        case 1: // 연속 공격 횟수 증가
        SkillAbilityComponent->Coefficient_Add(SpawnAmmoNum, AbilityData->Coefficient_X, NestingNum);
        SpawnAmmoNum = FMath::Min(3, SpawnAmmoNum);
        break;

        case 2: // 터렛 어그로
        Tags.Add(TEXT("Aggro"));
        break;

        case 3: // 파괴 기절
        StateTime = 0.0f;
        SkillAbilityComponent->Coefficient_Add(StateTime, AbilityData->Coefficient_X, NestingNum);
        bDeadStun = true;
        break;  
    }
}

void AArcaneTurret::CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum)
{
    Super::CheckPlatinumEnhance(AbilityNum, NestingNum);

    switch (AbilityNum)
    {
        case 1: // 충전횟수, 이미 처리
        break;

        case 2: // 강화 공격
        bEnhance = true;
        break;

        case 3: // 무적
        bGodMode = true;
        break;
    }
}
