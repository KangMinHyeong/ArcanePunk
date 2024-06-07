
#include "AnimInstance/AP_EnemyBossAnimInstance.h"

#include "Enemy/Enemy_Boss.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAP_EnemyBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
}

void UAP_EnemyBossAnimInstance::PlayMismatchedAttack_Montage()
{
    if(IsDead) return;
    Montage_Play(MismatchedAttack_Montage, 1.0f);
}

void UAP_EnemyBossAnimInstance::PlayKnockBackAttack_Montage()
{
    if(IsDead) return;
    Montage_Play(KnockBackAttack_Montage, 0.85f);
}

void UAP_EnemyBossAnimInstance::PlayStrongAttack_Montage()
{
    if(IsDead) return;
    Montage_Play(Strong_Montage, 1.65f);
}

void UAP_EnemyBossAnimInstance::PlaySpawnMonster_Montage()
{
    if(IsDead) return;
    Montage_Play(SpawnMonster_Montage);
}

void UAP_EnemyBossAnimInstance::PlayDrainMonster_Montage()
{
    if(IsDead) return;
    Montage_Play(DrainMonster_Montage);

    auto Boss = Cast<AEnemy_Boss>(TryGetPawnOwner());
    if(Boss) Boss->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UAP_EnemyBossAnimInstance::PlayRangeAttack1_Montage()
{
    if(IsDead) return;
    Montage_Play(RangeAttack1_Montage);
}

void UAP_EnemyBossAnimInstance::PlayRangeAttack2_Montage()
{
    if(IsDead) return;
    Montage_Play(RangeAttack2_Montage);
}

void UAP_EnemyBossAnimInstance::PlayPillarAttack_Montage()
{
    if(IsDead) return;
    Montage_Play(PillarAttack_Montage, 1.35f);
}

void UAP_EnemyBossAnimInstance::PlayPillarThrowing_Montage()
{
    if(IsDead) return;
    Montage_Play(PillarThrowing_Montage, 0.88f);
}

void UAP_EnemyBossAnimInstance::AnimNotify_MismatchedAttack()
{
    if(IsDead) return;
	if(!Montage_IsPlaying(MismatchedAttack_Montage))return;
    MismatchSection++;
    Montage_SetPlayRate(MismatchedAttack_Montage, 1.0f);
	Montage_JumpToSection(GetAttackMontageSectionName(MismatchSection), MismatchedAttack_Montage);
    if(MismatchSection == 3) MismatchSection = 1;
}

void UAP_EnemyBossAnimInstance::AnimNotify_AttackMove()
{
    auto Boss = Cast<AEnemy_Boss>(TryGetPawnOwner());
    if(Boss) { Boss->SetAttackMove(true); Boss->SetActorTickEnabled(true);}
}

void UAP_EnemyBossAnimInstance::AnimNotify_AttackMoveEnd()
{
    auto Boss = Cast<AEnemy_Boss>(TryGetPawnOwner());
    if(Boss) {Boss->SetAttackMove(false); Boss->SetActorTickEnabled(false);}
}

void UAP_EnemyBossAnimInstance::AnimNotify_KnockBackAttack()
{
    auto Boss = Cast<AEnemy_Boss>(TryGetPawnOwner());
    if(Boss) Boss->KnockBackActivate();
}

void UAP_EnemyBossAnimInstance::AnimNotify_SpawnFastSlash()
{
    auto Boss = Cast<AEnemy_Boss>(TryGetPawnOwner());
    if(Boss) Boss->SpawnFastSlash();
}

void UAP_EnemyBossAnimInstance::AnimNotify_SpawnMonster()
{
    auto Boss = Cast<AEnemy_Boss>(TryGetPawnOwner());
    if(Boss) Boss->SpawnLocationEffect();
}

void UAP_EnemyBossAnimInstance::AnimNotify_RangeAttack1()
{
    auto Boss = Cast<AEnemy_Boss>(TryGetPawnOwner());
    if(Boss) Boss->SpawnRangeAttack_1();
}

void UAP_EnemyBossAnimInstance::AnimNotify_RangeAttack2()
{
    auto Boss = Cast<AEnemy_Boss>(TryGetPawnOwner());
    if(Boss) Boss->SpawnRangeAttack_2();
}

void UAP_EnemyBossAnimInstance::AnimNotify_ThrowingPillar()
{
    auto Boss = Cast<AEnemy_Boss>(TryGetPawnOwner());
    if(Boss) Boss->OnThrowingPillar();
}

FName UAP_EnemyBossAnimInstance::GetAttackMontageSectionName(uint8 Section)
{
    if(Section >= 2 && Section <= 3)
    {
        return FName(*FString::Printf(TEXT("Attack%d"), Section));
    }
	return FName(TEXT(""));
}
