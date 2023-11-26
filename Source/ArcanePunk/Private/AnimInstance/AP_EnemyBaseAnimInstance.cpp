
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"

#include "Animation/AnimSequence.h"
#include "Enemy/Enemy_CharacterBase.h"

UAP_EnemyBaseAnimInstance::UAP_EnemyBaseAnimInstance()
{
}

void UAP_EnemyBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

	auto Character = Cast<AEnemy_CharacterBase>(TryGetPawnOwner());
	if (!Character) return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Character->GetVelocity().Size();

		// if(CurrentPawnSpeed < 1.0f)
		// {
		// 	UE_LOG(LogTemp, Display, TEXT("Your a"));
		// 	Character->GetMesh()->PlayAnimation(IdlePose, false);
		// }
		// else
		// {
		// 	UE_LOG(LogTemp, Display, TEXT("Your b"));
		// 	Character->GetMesh()->PlayAnimation(RunPose, false);
		// }

		// auto Character = Cast<ACharacter>(Pawn);
		// if (Character)
		// {
		// 	IsInAir = Character->GetMovementComponent()->IsFalling();
		// }
		
	}
}
bool UAP_EnemyBaseAnimInstance::IsRun()
{
	if(CurrentPawnSpeed >= 1.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
    return false;
}
