
#include "Components/Common/APCrowdControlComponent.h"

#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UAPCrowdControlComponent::UAPCrowdControlComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAPCrowdControlComponent::BeginPlay()
{
	Super::BeginPlay();
	CC_Priority.Init(false, 5);
	
}

void UAPCrowdControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UAPCrowdControlComponent::CalculateStateTime(ECharacterState UpdateState, FTimerHandle& StateTimerHandle,float StateTime)
{
	if(StateMap.Contains(UpdateState))
	{
		float CurrentStunTime = StateMap.FindOrAdd(UpdateState);
		float Origin = CurrentStunTime - (GetWorld()->GetTimerManager().GetTimerRate(StateTimerHandle) - GetWorld()->GetTimerManager().GetTimerElapsed(StateTimerHandle));
		if(StateTime > Origin)
		{
			GetWorld()->GetTimerManager().ClearTimer(StateTimerHandle);
		}
		else {return false;}
	}
	else
	{
		StateMap.Add(UpdateState, StateTime);
	}
	return true;
}

void UAPCrowdControlComponent::NormalState() // 후에 벡터로 관리 ?
{
	bool count = false;
	for(int32 i = 1; i<CC_Priority.Num(); i++)
	{
		if(CC_Priority[i])
		{
			count = true;
			SwitchingState(static_cast<ECharacterState>(i));
			break;
		}
	}
	if(!count) SwitchingState(ECharacterState::None);

}

void UAPCrowdControlComponent::KnockBackState(FVector KnockBackPoint, float KnockBackTime)
{
	if(!CalculateStateTime(ECharacterState::KnockBack, KnockBackTimerHandle, KnockBackTime)){return;}

	GetWorld()->GetTimerManager().SetTimer(KnockBackTimerHandle, this, &UAPCrowdControlComponent::KnockBackEnd, KnockBackTime, false);

	if(GetOwner()->ActorHasTag(TEXT("Player"))) {OnPlayerState(ECharacterState::KnockBack, true);}
	else if(GetOwner()->ActorHasTag(TEXT("Enemy"))) {OnEnemyState(ECharacterState::KnockBack, true);}

	
	auto Owner = Cast<ACharacter>(GetOwner()); if(!Owner) return;
	FVector KnockBackVec = (KnockBackPoint - Owner->GetActorLocation()) * FVector(1.0f, 1.0f, 0.0f);
	KnockBackVec = -( KnockBackVec / KnockBackVec.Size());
	DefaultSlip = Owner->GetCharacterMovement()->BrakingFrictionFactor;
	Owner->GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	Owner->LaunchCharacter(KnockBackVec * 1500.0f, true, true);
}

void UAPCrowdControlComponent::KnockBackEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(KnockBackTimerHandle);

	auto Owner = Cast<ACharacter>(GetOwner()); if(Owner) Owner->GetCharacterMovement()->BrakingFrictionFactor = DefaultSlip;

	ClearStopState();
	StateMap.Remove(ECharacterState::KnockBack);
	CC_Priority.Last((uint8)ECharacterState::KnockBack) = false;
	PlayStateEffect(ECharacterState::KnockBack, false);

	NormalState();
}

void UAPCrowdControlComponent::StunState(float StunTime)
{
	if(!CalculateStateTime(ECharacterState::Stun, StunTimerHandle, StunTime)){return;}
	GetWorld()->GetTimerManager().SetTimer(StunTimerHandle, this, &UAPCrowdControlComponent::StunEnd, StunTime, false);

	if(GetOwner()->ActorHasTag(TEXT("Player"))) {OnPlayerState(ECharacterState::Stun, true);}
	else if(GetOwner()->ActorHasTag(TEXT("Enemy"))) {OnEnemyState(ECharacterState::Stun, true);}

	PlayStateEffect(ECharacterState::Stun, true);
}

void UAPCrowdControlComponent::StunEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(StunTimerHandle);

	ClearStopState();
	StateMap.Remove(ECharacterState::Stun);
	CC_Priority.Last((uint8)ECharacterState::Stun) = false;
	PlayStateEffect(ECharacterState::Stun, false);

	NormalState();
}

void UAPCrowdControlComponent::SleepState()
{
	// CurrentState = 3;
	// bCanMove = false;
	// GetWorldTimerManager().SetTimer(SleepTimerHandle, this, &UAPCrowdControlComponent::NormalState, State_Time, false);
}

void UAPCrowdControlComponent::SlowState(float SlowCoefficient, float SlowTime)
{
	if(!CalculateStateTime(ECharacterState::Slow, SlowTimerHandle, SlowTime)){return;}
	GetWorld()->GetTimerManager().SetTimer(SlowTimerHandle, this, &UAPCrowdControlComponent::SlowEnd, SlowTime, false);

	if(GetOwner()->ActorHasTag(TEXT("Player"))) {OnPlayerState(ECharacterState::Slow, false);}
	else if(GetOwner()->ActorHasTag(TEXT("Enemy"))) {OnEnemyState(ECharacterState::Slow, false);}

	PlayStateEffect(ECharacterState::Slow, true);

	auto Owner = Cast<ACharacter>(GetOwner()); if(!Owner) return;
	DefaultSpeed = Owner->GetCharacterMovement()->MaxWalkSpeed;

	auto OwnerCharacter = Cast<AArcanePunkCharacter>(Owner); if(OwnerCharacter){ OwnerCharacter->SetCanJog(false);  DefaultSpeed = OwnerCharacter->GetDefaultSpeed();}
	
	Owner->GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed * SlowCoefficient;
}

void UAPCrowdControlComponent::SlowEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(SlowTimerHandle);

	auto Owner = Cast<ACharacter>(GetOwner()); if(Owner) Owner->GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(Owner); if(OwnerCharacter) OwnerCharacter->SetCanJog(true);

	StateMap.Remove(ECharacterState::Slow);
	CC_Priority.Last((uint8)ECharacterState::Slow) = false;
	PlayStateEffect(ECharacterState::Slow, false);

	NormalState();
}

void UAPCrowdControlComponent::OnPlayerState(ECharacterState UpdateState, bool IsStop)
{
	auto OwnerCharater = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharater) return;

	// 우선순위 체크
	if ((uint8)OwnerCharater->returnState() > (uint8)UpdateState || (uint8)OwnerCharater->returnState() == 0)
	{
		OwnerCharater->SetState(UpdateState);
		CC_Priority.Last((uint8)UpdateState) = true;
		if(IsStop) OwnerCharater->StopState.Add(true);
	}

}

void UAPCrowdControlComponent::OnEnemyState(ECharacterState UpdateState, bool IsStop)
{
	auto OwnerEnemy = Cast<AEnemy_CharacterBase>(GetOwner());
	if(!OwnerEnemy) return;

	if ((uint8)OwnerEnemy->ReturnState() > (uint8)UpdateState || (uint8)OwnerEnemy->ReturnState() == 0)
	{
		OwnerEnemy->SetState(UpdateState);
		CC_Priority.Last((uint8)UpdateState) = true;
		if(IsStop) {OwnerEnemy->StopState.Add(true); OwnerEnemy->OnCrowdControlCheck.Broadcast();}
	}
}

void UAPCrowdControlComponent::ClearStopState()
{
	if(GetOwner()->ActorHasTag(TEXT("Player"))) 
	{
		auto OwnerCharater = Cast<AArcanePunkCharacter>(GetOwner());
		if(!OwnerCharater) return;

		if(!OwnerCharater->StopState.IsEmpty()) OwnerCharater->StopState.Pop();
	}
	else if(GetOwner()->ActorHasTag(TEXT("Enemy"))) 
	{
		auto OwnerEnemy = Cast<AEnemy_CharacterBase>(GetOwner());
		if(!OwnerEnemy) return;

		if(!OwnerEnemy->StopState.IsEmpty()) {OwnerEnemy->StopState.Pop(); OwnerEnemy->OnCrowdControlCheck.Broadcast();}
	}
}

void UAPCrowdControlComponent::SwitchingState(ECharacterState UpdateState)
{
	if(GetOwner()->ActorHasTag(TEXT("Player"))) 
	{
		auto OwnerCharater = Cast<AArcanePunkCharacter>(GetOwner());
		if(OwnerCharater) OwnerCharater->SetState(UpdateState);

	}
	else if(GetOwner()->ActorHasTag(TEXT("Enemy"))) 
	{
		auto OwnerEnemy = Cast<AEnemy_CharacterBase>(GetOwner());
		if(OwnerEnemy) {OwnerEnemy->SetState(UpdateState); OwnerEnemy->OnCrowdControlCheck.Broadcast();}
	}
}

void UAPCrowdControlComponent::PlayStateEffect(ECharacterState UpdateState, bool IsPlay)
{
	auto OwnerCharater = Cast<AArcanePunkCharacter>(GetOwner());
	auto OwnerEnemy = Cast<AEnemy_CharacterBase>(GetOwner());

	switch (UpdateState)
	{
		case ECharacterState::KnockBack:
		break;
		
		case ECharacterState::Stun:
		if(OwnerCharater) {IsPlay ? OwnerCharater->GetStunEffect()->Activate() : OwnerCharater->GetStunEffect()->DeactivateImmediate(); }
		else if(OwnerEnemy) {IsPlay ? OwnerEnemy->GetStunEffect()->Activate() : OwnerEnemy->GetStunEffect()->DeactivateImmediate();}
		break;

		case ECharacterState::Sleep:
		break;
		
		case ECharacterState::Slow:
		break;
	}
}
