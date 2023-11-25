
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"

UAP_EnemyBaseAnimInstance::UAP_EnemyBaseAnimInstance()
{
}

void UAP_EnemyBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

	auto Character = TryGetPawnOwner();
	if (!Character) return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Character->GetVelocity().Size();
		// auto Character = Cast<ACharacter>(Pawn);
		// if (Character)
		// {
		// 	IsInAir = Character->GetMovementComponent()->IsFalling();
		// }
	}
}
