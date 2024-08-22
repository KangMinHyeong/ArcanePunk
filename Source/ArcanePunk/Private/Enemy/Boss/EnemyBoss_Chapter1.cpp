
#include "Enemy/Boss/EnemyBoss_Chapter1.h"

#include "Enemy/SkillActor/APEnemyAmmo.h"
#include "Enemy/SkillActor/APEnemyAttackRange.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Character/ArcanePunkCharacter.h"

AEnemyBoss_Chapter1::AEnemyBoss_Chapter1()
{
    AmmoSpawnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("AmmoSpawnComponent"));

    AmmoSpawnComponent->SetupAttachment(GetMesh(), TEXT("AmmoSpawnLocation"));
}

void AEnemyBoss_Chapter1::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyBoss_Chapter1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(IsDead()) return;

    if(bTraceAttack)
    {
        auto CurrentLocation = GetActorLocation();
        CurrentLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, TraceSpeed);
        SetActorLocation(CurrentLocation, true);
        if(abs(CurrentLocation.X - TargetLocation.X )< 1.0f && abs(CurrentLocation.Y - TargetLocation.Y) < 1.0f) 
        {
            GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &AEnemyBoss_Chapter1::OnOverlapping);
            bTraceAttack = false;
            SetCapsuleOverlap(false);
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
    auto Ammo = GetWorld()->SpawnActor<AAPEnemyAmmo>(AmmoClass, AmmoSpawnComponent->GetComponentLocation(), GetActorRotation());
    if(!Ammo) return;
    Ammo->SetOwner(this);
    Ammo->SetOwnerEnemy(Monster_RangeAttack1_Range, Monster_RangeAttack1_Radius);
}

void AEnemyBoss_Chapter1::RangeAttack_2()
{
}

void AEnemyBoss_Chapter1::RangeAttack_3()
{
}

void AEnemyBoss_Chapter1::TraceAttack_1()
{
    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBoss_Chapter1::OnOverlapping);
    bTraceAttack = true;
    SetCapsuleOverlap(true);
}

void AEnemyBoss_Chapter1::TraceAttack_2()
{
}

void AEnemyBoss_Chapter1::SpawnAttackRange(AActor* Target)
{
    FVector SpawnLoc = GetMesh()->GetComponentLocation();
    float Radius = 0.0f; float Range = 0.0f; 

    if(BossPhase == EBossPhase::Phase_1)
    {
        switch (CurrentPatterNum)
        {
        case 1:
            Radius = Monster_AttackRadius;
            Range = Monster_AttackRange_Plus + Radius;
            break;
        
        case 2:
            Radius = Monster_RangeAttack1_Radius;
            Range = Monster_RangeAttack1_Range + Radius;
            SpawnLoc.X = AmmoSpawnComponent->GetComponentLocation().X;
            SpawnLoc.Y = AmmoSpawnComponent->GetComponentLocation().Y;
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
            break;
        
        case 2:
            break;

        case 3:
            break;
        }
    }

    auto AttackRange = GetWorld()->SpawnActor<AAPEnemyAttackRange>(AttackRangeClass, SpawnLoc, GetActorRotation()); if(!AttackRange) return;
	AttackRange->SetDecalSize(Radius, Range, AttackRangeTime);
	
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