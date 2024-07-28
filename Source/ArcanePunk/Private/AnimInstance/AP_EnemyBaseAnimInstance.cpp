
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"

#include "Animation/AnimSequence.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Enemy/Enemy_ScoutDog.h"
#include "GameFramework/CharacterMovementComponent.h"

UAP_EnemyBaseAnimInstance::UAP_EnemyBaseAnimInstance()
{
}

void UAP_EnemyBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

	Enemy = Cast<AEnemy_CharacterBase>(TryGetPawnOwner());
	if (!Enemy.IsValid()) return;

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

bool UAP_EnemyBaseAnimInstance::IsRun()
{
	if(IsDead) return false;
	
	if(CurrentPawnSpeed >= 0.1f)
	{
		return true;
	}

    return false;
}

void UAP_EnemyBaseAnimInstance::PlayNormalAttack_Montage()
{
	if(IsDead || !Enemy.IsValid()) return;
    if(Enemy->IsHardCC()) return;
    Montage_Play(NormalAttack_Montage);
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

void UAP_EnemyBaseAnimInstance::PlayDetect_Montage()
{
	if(IsDead || !Enemy.IsValid()) return;
    if(Enemy->IsHardCC()) return;
    Montage_Play(Detect_Montage);
}

UAnimMontage* UAP_EnemyBaseAnimInstance::PlayRandomIdle_Montage()
{
	if(IsDead || !Enemy.IsValid()) return nullptr;
    if(Enemy->IsHardCC()) return nullptr;
    if(CurrentPawnSpeed >= 0.1f) return nullptr;

	int32 Index = FMath::RandRange(0, Idle_Montages.Num()-1); Index = FMath::Max(0,Index);
	Montage_Play(Idle_Montages[Index]);
	return Idle_Montages[Index];
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