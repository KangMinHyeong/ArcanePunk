
#include "Enemy/Enemy_Boss.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AnimInstance/AP_EnemyBossAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Character/ArcanePunkCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "SpawnPoint/APSpawnPointBase.h"
#include "Engine/DamageEvents.h"
#include "Skill/SwordImpact.h"
#include "UserInterface/APHUD.h"
#include "UserInterface/Enemy/APEnemyHP.h"
#include "Enemy/AIController/EnemyBossBaseAIController.h"
#include "Components/Common/APSpawnMonsterComponent.h"

AEnemy_Boss::AEnemy_Boss()
{
    PrimaryActorTick.bCanEverTick = true;
    PatternMaterial.SetNum(5);

    SpawnMonsterComp = CreateDefaultSubobject<UAPSpawnMonsterComponent>(TEXT("SpawnMonsterComp"));
    RushEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RushEffect"));
    RushTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RushTrigger"));
    

    RushEffect->SetupAttachment(GetMesh());
    RushTrigger->SetupAttachment(GetMesh());
}

void AEnemy_Boss::BeginPlay()
{
    Super::BeginPlay();

    MonsterAIController = Cast<AEnemyBaseAIController>(GetController());
    if(MonsterAIController) MonsterAIController->UnPossessing();

    InitPatternNums(); 
    RushTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BindMontageEnd();
    BindRushAttack();
    DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;
    // SetHPUI();
}

void AEnemy_Boss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ActiveRushEffect();
    OnAttackMove(DeltaTime);
}

void AEnemy_Boss::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

bool AEnemy_Boss::OnPossessing()
{
	if(!MonsterAIController) return false;

	MonsterAIController->OnPossess(this);
	return true;
}

// Montage Bind Func Start
void AEnemy_Boss::BindMontageEnd()
{
	OwnerAnim = Cast<UAP_EnemyBossAnimInstance>(GetMesh()->GetAnimInstance()); if(!OwnerAnim.IsValid()) return;

	// MontageEnd
	OwnerAnim->OnMontageEnded.AddDynamic(this, &AEnemy_Boss::BossMontageEnded);
}

void AEnemy_Boss::BossMontageEnded(UAnimMontage *Montage, bool bInterrupted)
{
    if(!OwnerAnim.IsValid()) return;
	
	if(Montage == OwnerAnim->RushAttack_Montage) OnRushAttack_MontageEnd();
    else if(Montage == OwnerAnim->MismatchedAttack_Montage) OnMismatchedAttack_MontageEnd();
    else if(Montage == OwnerAnim->KnockBackAttack_Montage) OnKnockBackAttack_MontageEnd();
    else if(Montage == OwnerAnim->Strong_Montage) OnStrongAttack_MontageEnd();
    else if(Montage == OwnerAnim->SpawnMonster_Montage) OnSpawnMonster_MontageEnd();
    else if(Montage == OwnerAnim->DrainMonster_Montage) OnDrainMonster_MontageEnd();
    else if(Montage == OwnerAnim->RangeAttack1_Montage) OnRangeAttack1_MontageEnd();
    else if(Montage == OwnerAnim->RangeAttack2_Montage) OnRangeAttack2_MontageEnd();
}

void AEnemy_Boss::OnRushAttack_MontageEnd()
{
}

void AEnemy_Boss::OnMismatchedAttack_MontageEnd()
{
}

void AEnemy_Boss::OnKnockBackAttack_MontageEnd()
{
    bKnockBackAttack = false;
    Monster_AttackRadius = Monster_AttackRadius * (1/KnockBackRangeCoefficient);
    Monster_AttackRange = Monster_AttackRange * (1/KnockBackRangeCoefficient);
    GetMesh()->SetMaterial(0,DefaultMaterial);
}

void AEnemy_Boss::OnStrongAttack_MontageEnd()
{
    GetWorldTimerManager().SetTimer(StopTimerHandle, this, &AEnemy_Boss::StopEnd, StopTime, false);
    if(PatternMaterial.Num() >= 5) GetMesh()->SetMaterial(0,PatternMaterial.Last(2));
}

void AEnemy_Boss::OnSpawnMonster_MontageEnd()
{
}

void AEnemy_Boss::OnDrainMonster_MontageEnd()
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AEnemy_Boss::OnRangeAttack1_MontageEnd()
{
    GetMesh()->SetMaterial(0,DefaultMaterial);
}

void AEnemy_Boss::OnRangeAttack2_MontageEnd()
{
    GetMesh()->SetMaterial(0,DefaultMaterial);
}
// Montage Bind Func End

//Rush Attack Func Start
void AEnemy_Boss::RushAttackStart()
{
    GetWorldTimerManager().SetTimer(RushAttackTimerHandle, this, &AEnemy_Boss::RushAttackEnd, RushAttackTime, false);
    bRushAttack = true;
    GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed*2;
    if(PatternMaterial.Num() >= 5) GetMesh()->SetMaterial(0,PatternMaterial.Last(4));
}

void AEnemy_Boss::RushAttackEnd()
{
    bRushAttack = false;
    GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
    GetMesh()->SetMaterial(0,DefaultMaterial);
	RushTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetWorldTimerManager().ClearTimer(RushAttackTimerHandle);
}

void AEnemy_Boss::BindRushAttack()
{
    RushTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RushTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Boss::ActiveRushTrigger);
}

void AEnemy_Boss::ActiveRushEffect()
{
    if(GetVelocity().Size() >= 950.0f)
    {
        RushTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TestRushEffect, RushEffect->GetComponentLocation(), RushEffect->GetComponentRotation());
    }
    else { RushTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);}
}

void AEnemy_Boss::ActiveRushTrigger(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	auto MyOwnerInstigator = GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	
	if (OtherActor && OtherActor != this)
	{
        DistinctHitPoint(SweepResult.Location, OtherActor);
		UGameplayStatics::ApplyDamage(OtherActor, Monster_ATK * RushCoefficient, MyOwnerInstigator, this, DamageTypeClass);
        OnPlayerKnockBack(OtherActor, KnockBackDist, RushKnockBackTime);
	}
    RushAttackEnd();
}
// Rush Attack Func End

// KnockBack Attack Func Start
void AEnemy_Boss::KnockBackAttackReady()
{
    if(PatternMaterial.Num() >= 5) GetMesh()->SetMaterial(0,PatternMaterial.Last(3));
}

void AEnemy_Boss::KnockBackAttack()
{
    Monster_AttackRadius = Monster_AttackRadius * KnockBackRangeCoefficient;
    Monster_AttackRange = Monster_AttackRange * KnockBackRangeCoefficient;
    if(!OwnerAnim.IsValid()) return;
    OwnerAnim->PlayKnockBackAttack_Montage();
}

void AEnemy_Boss::KnockBackActivate()
{
    bKnockBackAttack = true;
    UNiagaraFunctionLibrary::SpawnSystemAttached(KnockBackSlash, GetMesh(), TEXT("KnockBackSlashEffect"), GetActorLocation(), GetActorRotation() + KnockBackSlashRotator, EAttachLocation::KeepWorldPosition, true);
    NormalAttack();
}
// KnockBack Attack Func End

// Strong Attack Func Start
void AEnemy_Boss::SpawnFastSlash()
{
    UNiagaraFunctionLibrary::SpawnSystemAttached(FastSlash, GetMesh(), TEXT("FastSlashEffec"), GetActorLocation() + GetActorForwardVector()*35.0f, GetActorRotation() - FastSlashRotator, EAttachLocation::KeepWorldPosition, true);
}
// Strong Attack Func End

// SpawnMonster Func Start
void AEnemy_Boss::SpawnLocationEffect()
{
    SpawnMonsterComp->SpawnLocation(SpawnPointClass, SpawnMonsterNum, SpawnLocation);
    GetWorldTimerManager().SetTimer(SpawnMonsterTimerHandle, this, &AEnemy_Boss::SpawnMonster, SpawnMonsterTime, false);
}

void AEnemy_Boss::SpawnMonster()
{
    SpawnMonsterComp->SpawnMonsterFromLocation(SpawnMonsterClass, SpawnLocation, MonsterArr);
    GetWorldTimerManager().SetTimer(DrainMonsterTimerHandle, this, &AEnemy_Boss::DrainMonster, DrainMonsterTime, false);
    GetWorldTimerManager().ClearTimer(SpawnMonsterTimerHandle);
}

void AEnemy_Boss::DrainMonster()
{
    if(MonsterArr.IsEmpty()) return;

    if(OwnerAnim.IsValid()) OwnerAnim->PlayDrainMonster_Montage();
    UNiagaraFunctionLibrary::SpawnSystemAttached(DrainEffect, GetMesh(), TEXT("DrainEffect"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
    
    while(!MonsterArr.IsEmpty())
    {
        if(!MonsterArr.Top()->IsDead())
        {
            MonsterArr.Top()->Destroy();
            HP = FMath::Min(MaxHP, HP+100.0f);
            UE_LOG(LogTemp, Display, TEXT("Monster HP : %f"), HP);
            OnEnemyHPChanged.Broadcast();
        }
        MonsterArr.Pop();
    }
    GetWorldTimerManager().ClearTimer(DrainMonsterTimerHandle);
}
// SpawnMonster Func End

// RangeAttack_1 Func Start
void AEnemy_Boss::RangeAttack_1_Ready()
{
    if(PatternMaterial.Num() >= 5) GetMesh()->SetMaterial(0,PatternMaterial.Top());
    GetWorldTimerManager().SetTimer(RangeAttack_1_ReadyTimerHandle, this, &AEnemy_Boss::RangeAttack_1, RangeAttack_1_ReadyTime, false);
}

void AEnemy_Boss::RangeAttack_1()
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.bNoFail = true;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    if(!RandomRangeAttack)
    {
        auto Character = Cast<AArcanePunkCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)); if(!Character) return;
        auto SpawnPoint = GetWorld()->SpawnActor<AAPSpawnPointBase>(SpawnRangeAttackPointClass, Character->GetActorLocation(), GetActorRotation(), SpawnParams);
        if(SpawnPoint) SpawnRangeAttackLocation.Add(SpawnPoint);
    }
    else
    {
        for(int32 i = 0; i<RangeAttackNum; i++)
        {
            AAPSpawnPointBase* SpawnPoint = nullptr;
            while(!SpawnPoint)
            {
                float IsSpawnVec_X = FMath::RandRange(GetActorLocation().X - RangeAttackDist,  GetActorLocation().X + RangeAttackDist);
                float IsSpawnVec_Y = FMath::RandRange(GetActorLocation().Y - RangeAttackDist,  GetActorLocation().Y + RangeAttackDist);
                SpawnPoint = GetWorld()->SpawnActor<AAPSpawnPointBase>(SpawnRangeAttackPointClass, FVector(IsSpawnVec_X, IsSpawnVec_Y, GetActorLocation().Z), GetActorRotation(), SpawnParams);
            }
            SpawnRangeAttackLocation.Add(SpawnPoint);
        }
    }
    if(OwnerAnim.IsValid()) OwnerAnim->PlayRangeAttack1_Montage();
}

void AEnemy_Boss::SpawnRangeAttack_1()
{
    while(!SpawnRangeAttackLocation.IsEmpty())
    {
        FVector Location = FVector(SpawnRangeAttackLocation.Top()->GetActorLocation().X, SpawnRangeAttackLocation.Top()->GetActorLocation().Y, SpawnRangeAttackLocation.Top()->GetActorLocation().Z - SpawnRangeAttackLocation.Top()->GetBlockingArea()->GetScaledCapsuleHalfHeight());
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RangeAttack_1_Impact, Location , GetActorRotation());
        FHitResult HitResult; FVector HitVector; 
        if(AttackTrace(HitResult, HitVector, true, SpawnRangeAttackLocation.Top()->GetBlockingArea()->GetScaledCapsuleRadius(), Location, SpawnRangeAttackLocation.Top()->GetActorLocation()))
        {
            if(AActor* Actor = HitResult.GetActor())
            {
                float Damage= Monster_ATK * RangeAttack_1_Coefficient;
                FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
                AController* MyController = Cast<AController>(GetController());
                if(MyController == nullptr) return;
                DistinctHitPoint(HitResult.Location, Actor);
                Actor->TakeDamage(Damage, myDamageEvent, MyController, this);
                OnPlayerStun(Actor, StunTime);
            }
        }
        SpawnRangeAttackLocation.Top()->Destroy();
        SpawnRangeAttackLocation.Pop();
    }
    GetWorldTimerManager().ClearTimer(RangeAttack_1_ReadyTimerHandle);
}
// RangeAttack_1 Func End

// RangeAttack_2 Func Start
void AEnemy_Boss::RangeAttack_2_Ready()
{
    if(PatternMaterial.Num() >= 5) GetMesh()->SetMaterial(0,PatternMaterial.Last(1));
    GetWorldTimerManager().SetTimer(RangeAttack_2_ReadyTimerHandle, this, &AEnemy_Boss::RangeAttack_2, RangeAttack_2_ReadyTime, false);
}

void AEnemy_Boss::RangeAttack_2()
{
    GetWorldTimerManager().ClearTimer(RangeAttack_2_ReadyTimerHandle);
    if(OwnerAnim.IsValid()) OwnerAnim->PlayRangeAttack2_Montage();
}

void AEnemy_Boss::SpawnRangeAttack_2()
{
    if(RA_Num < 2)
    {
        if(RA_Num == 0) {ForwardDirection = true; DiagonalDirection = false;}
        else {ForwardDirection = false; DiagonalDirection = true;}
        RA_Num++;
    }
    else
    {
        ForwardDirection = true;
        DiagonalDirection = true;
        RA_Num = 0;
    }
    
    if(ForwardDirection) { OnSwordImpactSpawn();}
    
    if(DiagonalDirection) { OnSwordImpactSpawn(45.0f);}   
}

void AEnemy_Boss::OnSwordImpactSpawn(float AddAngle)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.bNoFail = true;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for(int32 i = 0; i<4; i++)
    {
        auto SpawnRangeAttack2 = GetWorld()->SpawnActor<ASwordImpact>(RangeAttack2Class, GetActorLocation(), FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + 90.0f * i + AddAngle, GetActorRotation().Roll), SpawnParams);
    }
}
// RangeAttack_2 Func End

// HPUI Set Start
bool AEnemy_Boss::SetHPUI()
{ 
    PlayerHUD = Cast<AAPHUD>(NewObject<AHUD>(this, UGameplayStatics::GetGameMode(GetWorld())->HUDClass));
    if(!PlayerHUD.IsValid()) return false;
    PlayerHUD->SetBossHPUI();
	UAPEnemyHP* HPUI = Cast<UAPEnemyHP>(PlayerHUD->GetBossHPUI());
	if(HPUI) HPUI->SetEnemy(this);
	OnEnemyHPChanged.Broadcast();
    return true;
}

void AEnemy_Boss::EnemyDestroyed()
{
    Super::EnemyDestroyed();
    if(!PlayerHUD.IsValid()) return;
    UAPEnemyHP* HPUI = Cast<UAPEnemyHP>(PlayerHUD->GetBossHPUI());
	if(HPUI) HPUI->RemoveFromParent();
}
// HPUI Set End

// Attack Move Func Start
void AEnemy_Boss::OnAttackMove(float DeltaTime)
{
    if(bAttackMove)
    {
        SetActorLocation(GetActorLocation() + GetActorForwardVector() * AttackMoveSpeed * DeltaTime);
        OnHitPlayerMove(GetActorForwardVector() * AttackMoveSpeed * DeltaTime * 1.1f);
    }
}

void AEnemy_Boss::OnHitPlayerMove(FVector MoveLocation)
{
    FHitResult HitResult; FVector HitVector;
    if(!AttackTrace(HitResult, HitVector)) return;
    auto Character = Cast<AArcanePunkCharacter>(HitResult.GetActor());
    if(Character) Character->SetActorLocation(Character->GetActorLocation() + MoveLocation);
}
// Attack Move Func End

// Stop Func Start
void AEnemy_Boss::StopEnd()
{
    GetMesh()->SetMaterial(0,DefaultMaterial);
    GetWorldTimerManager().ClearTimer(StopTimerHandle);
}
// Stop Func End

// Pattern Num Func Start
void AEnemy_Boss::InitPatternNums()
{
    for(int32 i = 1; i<=BossSkillNum; i++)
    {
        PatternNums.Add(i);
    }
    
    int32 LastIndex = PatternNums.Num() - 1;

    for (int32 i = 0; i <= LastIndex; i += 1) 
    {
        int32 Index = FMath::RandRange(i, LastIndex);
        if (i == Index) 
        {
            continue;
        }
        PatternNums.Swap(i, Index);
    }
}

int32 AEnemy_Boss::GetPatternNum()
{
    if(PatternNums.IsEmpty())
    {
        InitPatternNums();
    }
    int32 Num = PatternNums.Top();
    PatternNums.Pop();
    return Num;
}
// Pattern Num Func End