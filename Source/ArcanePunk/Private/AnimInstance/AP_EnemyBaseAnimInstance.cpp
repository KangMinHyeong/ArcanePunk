
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"

#include "Animation/AnimSequence.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Enemy/Enemy_ScoutDog.h"
#include "Enemy/Enemy_BossBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UAP_EnemyBaseAnimInstance::UAP_EnemyBaseAnimInstance()
{
}

void UAP_EnemyBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

	if(!Enemy.IsValid()) return;

	if (!Enemy->IsDead())
	{
		CurrentPawnSpeed = Enemy->GetVelocity().Size();

		// auto Character = Cast<ACharacter>(Pawn);
		// if (Character)
		// {
		// 	IsInAir = Character->GetMovementComponent()->IsFalling();
		// }
		
	}
}

void UAP_EnemyBaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Enemy = Cast<AEnemy_CharacterBase>(TryGetPawnOwner());
}

bool UAP_EnemyBaseAnimInstance::IsRun()
{
	if(IsDead) return false;
	
	if(CurrentPawnSpeed >= 0.1f)
	{
		return true;
	}

    return false;
}

float UAP_EnemyBaseAnimInstance::PlayNormalAttack_Montage()
{
	if(IsDead || !Enemy.IsValid()) return 0.0f;
    if(Enemy->IsHardCC()) return 0.0f;
    return Montage_Play(NormalAttack_Montage);
}

void UAP_EnemyBaseAnimInstance::PlayDeath_Montage()
{
    Montage_Play(Death_Montage);
}

void UAP_EnemyBaseAnimInstance::PlayHit_Montage()
{
	if (IsDead || !Enemy.IsValid()) return;
	if (Enemy->IsHardCC()) return;
	Montage_Play(Hit_Montage);
	Enemy->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

float UAP_EnemyBaseAnimInstance::PlayDetect_Montage()
{
	if(IsDead || !Enemy.IsValid()) return 0.0f;
    if(Enemy->IsHardCC()) return 0.0f;
    return Montage_Play(Detect_Montage);
}

float UAP_EnemyBaseAnimInstance::PlayRandomIdle_Montage()
{
	if(IsDead || !Enemy.IsValid()) return 0.0f;
    if(Enemy->IsHardCC()) return 0.0f;
    if(CurrentPawnSpeed >= 0.1f) return 0.0f;

	int32 Index = FMath::RandRange(0, Idle_Montages.Num()-1); Index = FMath::Max(0,Index);
	return Montage_Play(Idle_Montages[Index]);
}

float UAP_EnemyBaseAnimInstance::Play_Phase1Attack_Montage(uint8 Index)
{
	if(IsDead || !Enemy.IsValid()) return 0.0f;
    if(Enemy->IsHardCC()) return 0.0f;

	Index--;
	return Montage_Play(Phase1_Attacks[Index]);
}

float UAP_EnemyBaseAnimInstance::Play_Phase2Attack_Montage(uint8 Index)
{
	if(IsDead || !Enemy.IsValid()) return 0.0f;
    if(Enemy->IsHardCC()) return 0.0f;

	Index--;
	return Montage_Play(Phase2_Attacks[Index]);
}

void UAP_EnemyBaseAnimInstance::AnimNotify_NormalAttack()
{
	if(IsDead || !Enemy.IsValid()) return;
	if(Enemy->IsHardCC()) return;
	Enemy->NormalAttack();
}

// ScoutDog
void UAP_EnemyBaseAnimInstance::AnimNotify_JumpEffect()
{
    if(IsDead || !Enemy.IsValid()) return;
	if(Enemy->IsHardCC()) return;

	auto ScoutDog = Cast<AEnemy_ScoutDog>(Enemy); if(!ScoutDog) return;
	ScoutDog->SpawnJumpEffect();
}

void UAP_EnemyBaseAnimInstance::AnimNotify_LeapFoward()
{
    if(IsDead || !Enemy.IsValid()) return;
	if(Enemy->IsHardCC()) return;

    auto ScoutDog = Cast<AEnemy_ScoutDog>(Enemy); if(!ScoutDog) return;
	ScoutDog->LeapFoward();
}

void UAP_EnemyBaseAnimInstance::AnimNotify_LeapEnd()
{
    if(IsDead || !Enemy.IsValid()) return;
	if(Enemy->IsHardCC()) return;

    auto ScoutDog = Cast<AEnemy_ScoutDog>(Enemy); if(!ScoutDog) return;
	ScoutDog->AttackCondition(false);
}

void UAP_EnemyBaseAnimInstance::AnimNotify_RangeAttack_1()
{
	if(IsDead || !Enemy.IsValid()) return;
	if(Enemy->IsHardCC()) return;

    auto Boss = Cast<AEnemy_BossBase>(Enemy); if(!Boss) return;
	Boss->RangeAttack_1();
}

void UAP_EnemyBaseAnimInstance::AnimNotify_TraceAttack_1()
{
	if(IsDead || !Enemy.IsValid()) return;
	if(Enemy->IsHardCC()) return;

    auto Boss = Cast<AEnemy_BossBase>(Enemy); if(!Boss) return;
	Boss->TraceAttack_1();
}