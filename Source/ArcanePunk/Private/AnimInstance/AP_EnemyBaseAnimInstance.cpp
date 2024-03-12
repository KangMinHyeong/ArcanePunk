
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"

#include "Animation/AnimSequence.h"
#include "Enemy/Enemy_CharacterBase.h"

UAP_EnemyBaseAnimInstance::UAP_EnemyBaseAnimInstance()
{
}

void UAP_EnemyBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

	Enemy = Cast<AEnemy_CharacterBase>(TryGetPawnOwner());
	if (!Enemy.IsValid()) return;

	IsDead = Enemy->IsDead();
	if (!IsDead)
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
	else
	{
		return false;
	}
    return false;
}

void UAP_EnemyBaseAnimInstance::PlayNormalAttack_Montage()
{
	if(IsDead || !Enemy.IsValid()) return;
    if(Enemy->IsHardCC()) return;
    Montage_Play(NormalAttack_Montage);
}

void UAP_EnemyBaseAnimInstance::AnimNotify_NormalAttack()
{
	if(IsDead || !Enemy.IsValid()) return;
	if(Enemy->IsHardCC()) return;
	Enemy->NormalAttack();
}