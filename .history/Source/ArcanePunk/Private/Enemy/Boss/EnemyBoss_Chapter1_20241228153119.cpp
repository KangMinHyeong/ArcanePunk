
#include "Enemy/Boss/EnemyBoss_Chapter1.h"

#include "Enemy/SkillActor/APEnemyAmmo.h"
#include "Enemy/SkillActor/APEnemyAttackRange.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Character/ArcanePunkCharacter.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/APGameModeBattleStage.h"
#include "Enemy/SkillActor/APEnemyRescueBoat.h"

AEnemyBoss_Chapter1::AEnemyBoss_Chapter1()
{
    AmmoSpawnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("AmmoSpawnComponent"));

    AmmoSpawnComponent->SetupAttachment(GetMesh(), TEXT("AmmoSpawnLocation"));
}

void AEnemyBoss_Chapter1::BeginPlay()
{
    Super::BeginPlay();

}

void AEnemyBoss_Chapter1::EnemyDeadMotion()
{
    if(!EnemyAnim.IsValid()) return;
    EnemyAnim->PlayDeath_Montage();

    auto RescueBoat = GetWorld()->SpawnActor<AAPEnemyRescueBoat>(RescueBoatClass, GetActorLocation() + RescueAddPoint, GetActorRotation()); if(!RescueBoat) return;
    RescueBoat->SetRescuePoint(this);
}

void AEnemyBoss_Chapter1::RescueEnemy()
{
    if(!EnemyAnim.IsValid()) return;
    EnemyAnim->PlayRescue_Montage();
    RescueLocation = GetActorLocation() + FVector(0.0f, 0.0f, 220.0f);
}

void AEnemyBoss_Chapter1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bRescue)
    {
        auto CurrentLocation = GetActorLocation();
        CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, RescueLocation, DeltaTime, 285.0f);
        SetActorLocation(CurrentLocation);
        if((CurrentLocation - RescueLocation).Size() < 1.0f) DissolveEnemy(DeltaTime);
    }

    if(IsDead()) return;

    if(bTraceAttack)
    {
        auto CurrentLocation = GetActorLocation();
        CurrentLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, TraceSpeed * TraceSpeedCoeff);
        SetActorLocation(CurrentLocation, true);
        if(abs(CurrentLocation.X - TargetLocation.X )< 28.0f && abs(CurrentLocation.Y - TargetLocation.Y) < 28.0f) TraceSpeedCoeff = 100.0f;
        if(abs(CurrentLocation.X - TargetLocation.X )< 5.0f && abs(CurrentLocation.Y - TargetLocation.Y) < 5.0f) 
        {
            TraceSpeedCoeff = 1.0f;
            GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &AEnemyBoss_Chapter1::OnOverlapping);
            bTraceAttack = false;
            SetCapsuleOverlap(false);
            if(EnemyAnim.IsValid()) EnemyAnim->Montage_JumpToSection(TEXT("End"), EnemyAnim->Phase1_Attacks[2]);
        }
    }
}

void AEnemyBoss_Chapter1::MeleeAttack_1()
{
}

void AEnemyBoss_Chapter1::MeleeAttack_2()
{
}

void AEnemyBoss_Chapter1::RangeAttack_1()
{    
    if(SpawnRot.IsEmpty()) return;
    GetWorldTimerManager().ClearTimer(RangeTimerHandle);
    auto Ammo = GetWorld()->SpawnActor<AAPEnemyAmmo>(AmmoClass, AmmoSpawnComponent->GetComponentLocation(), SpawnRot.Top()); if(!Ammo) return;
    Ammo->SetOwner(this);
    Ammo->SetOwnerEnemy(Monster_RangeAttack_Range-50.0f, Monster_RangeAttack_Radius);
    // SpawnRot.Pop();
}

void AEnemyBoss_Chapter1::RangeAttack_2()
{
    RangeAttack_1();
    
    if(!EnemyAnim.IsValid()) return;
    if(RangeAttackInform.Num() >= 1 && EnemyAnim->Montage_GetCurrentSection() == "FireTrigger")
    GetWorldTimerManager().SetTimer(RangeTimerHandle, this, &AEnemyBoss_Chapter1::RangeAttack_2 , RangeAttackInform[1].Frequency, false);
}

void AEnemyBoss_Chapter1::RangeAttack_3()
{
    RangeAttack_1();

    if(!EnemyAnim.IsValid()) return;
    if(RangeAttackInform.Num() >= 2 && EnemyAnim->Montage_GetCurrentSection() == "FireTrigger")
    GetWorldTimerManager().SetTimer(RangeTimerHandle, this, &AEnemyBoss_Chapter1::RangeAttack_3 , RangeAttackInform[2].Frequency, false);
}

void AEnemyBoss_Chapter1::TraceAttack_1()
{
    GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &AEnemyBoss_Chapter1::OnOverlapping);
    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBoss_Chapter1::OnOverlapping);
    bTraceAttack = true;
    SetCapsuleOverlap(true);
}

void AEnemyBoss_Chapter1::TraceAttack_2()
{
    bJump = !bJump;
    if(!bJump)
    {
        SetActorLocation(TargetLocation);
    }
    SetActorHiddenInGame(bJump);
    SetCapsuleOverlap(bJump);
}

void AEnemyBoss_Chapter1::AroundDamage()
{
    float Damage = TotalStatus.StatusData.ATK * CriticalCalculate();
	FHitResult HitResult;
	FVector HitVector;
	FVector Start = GetMesh()->GetComponentLocation();
	if(AttackTrace(HitResult, HitVector, Monster_JumpAttack_Range, Start, GetActorLocation()))
	{
		if(AActor* Actor = HitResult.GetActor())
		{
			FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
			AController* MyController = Cast<AController>(GetController());
			if(MyController == nullptr) return;
			DistinctHitPoint(HitResult.Location, Actor);
			Actor->TakeDamage(Damage, myDamageEvent, MyController, this);
			if(bKnockBackAttack) OnPlayerKnockBack(Actor, KnockBackDist, KnockBackTime);
		}
	}
}

void AEnemyBoss_Chapter1::SpawnAttackRange(AActor* Target, float WaitTime)
{
    FVector SpawnLoc = GetMesh()->GetComponentLocation();
    SpawnRot.Empty(); SpawnRot.Emplace(GetActorRotation());
    float Radius = 0.0f; float Range = 0.0f; 
    auto RangeClass = AttackRangeClass; 
    bool bSquare = true;
    
    AAPEnemyAttackRange* AttackRange = nullptr;

    if(BossPhase == EBossPhase::Phase_1)
    {
        switch (CurrentPatterNum)
        {
        case 1:
            Radius = Monster_AttackRadius;
            Range = Monster_AttackRange_Plus + Radius;
            break;
        
        case 2:
            Radius = Monster_RangeAttack_Radius;
            Range = Monster_RangeAttack_Range + Radius;
            SpawnLoc.X = AmmoSpawnComponent->GetComponentLocation().X;
            SpawnLoc.Y = AmmoSpawnComponent->GetComponentLocation().Y;  
            
            SetRangeSpawnRot(0);
            break;

        case 3:
            Radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
            Range = Monster_TraceAttack1_Range + GetTargetDist2D(Target) + Radius; 
            TargetLocation = GetActorLocation() + GetActorForwardVector() * Range;
            break;
        }
    }
    else if(BossPhase == EBossPhase::Phase_2)
    {
        switch (CurrentPatterNum)
        {
        case 1:
            Radius = Monster_AttackRadius;
            Range = Monster_AttackRange_Plus + Radius;
            break;
        
        case 2:
            Radius = Monster_RangeAttack_Radius;
            Range = Monster_RangeAttack_Range + Radius;
            SpawnLoc.X = AmmoSpawnComponent->GetComponentLocation().X;
            SpawnLoc.Y = AmmoSpawnComponent->GetComponentLocation().Y;
            
            SetRangeSpawnRot(1);
            break;

        case 3:
            Radius = Monster_JumpAttack_Range;
            Range = Monster_JumpAttack_Range;
            TargetLocation = Target->GetActorLocation(); TargetLocation.Z = GetActorLocation().Z;
            SpawnLoc.X = TargetLocation.X;
            SpawnLoc.Y = TargetLocation.Y;
            RangeClass = AttackRangeClass_2;
            bSquare = false;
            break;

        case 4:
            Radius = Monster_RangeAttack_Radius;
            Range = Monster_RangeAttack_Range + Radius;
            SpawnLoc.X = AmmoSpawnComponent->GetComponentLocation().X;
            SpawnLoc.Y = AmmoSpawnComponent->GetComponentLocation().Y;
            SetRangeSpawnRot(2);
            break;
        }
    }
    
    for(auto Rot : SpawnRot)
    {
        AttackRange = GetWorld()->SpawnActor<AAPEnemyAttackRange>(RangeClass, SpawnLoc, Rot); if(!AttackRange) return;
	    AttackRange->SetDecalSize(Radius, Range, WaitTime, bSquare);
    }	
}

bool AEnemyBoss_Chapter1::IsDead()
{
    bool bResult = Super::IsDead();

    auto GM = Cast<AAPGameModeBattleStage>(UGameplayStatics::GetGameMode(GetWorld()));

    if(GM)
    GetWorldTimerManager().SetTimer(EndTimerHandle, GM, &AAPGameModeBattleStage::OnEndedGame , 5.0f, false);

    return bResult;
}

void AEnemyBoss_Chapter1::OnOverlapping(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto Player = Cast<AArcanePunkCharacter>(OtherActor); if(!Player) return;

    GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &AEnemyBoss_Chapter1::OnOverlapping);
    
    float Damage = TotalStatus.StatusData.ATK * CriticalCalculate();
	FVector HitVector;
    FPointDamageEvent myDamageEvent(Damage, SweepResult, HitVector, nullptr);
	AController* MyController = Cast<AController>(GetController());
	DistinctHitPoint(Player->GetActorLocation(), Player);
    Player->TakeDamage( Damage, myDamageEvent, MyController, this);
	if(bKnockBackAttack) OnPlayerKnockBack(Player, KnockBackDist, KnockBackTime);
}

void AEnemyBoss_Chapter1::RemoveHPUI()
{
}
