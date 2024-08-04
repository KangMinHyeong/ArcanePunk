
#include "Components/Common/APCrowdControlComponent.h"

#include "Character/APCharacterBase.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Skill/Sub/APFreezing.h"

UAPCrowdControlComponent::UAPCrowdControlComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAPCrowdControlComponent::BeginPlay()
{
	Super::BeginPlay();
	CC_Priority.Init(false, 6);
}

void UAPCrowdControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPCrowdControlComponent::BindCrowdComp()
{
	auto Owner = Cast<AAPCharacterBase>(GetOwner()); if(!Owner) return;

	Owner->OnUpdateOriginStatus.AddDynamic(this, &UAPCrowdControlComponent::UpdateStatus);
}

void UAPCrowdControlComponent::UpdateStatus()
{
	auto Owner = Cast<AAPCharacterBase>(GetOwner()); if(!Owner) return;
	auto PD = Owner->GetPlayerStatus_Origin(); 
	PD.StatusData.MoveSpeed = PD.StatusData.MoveSpeed * CurrentSlowCoefficient * CurrentFastCoefficient;
	Owner->GetCharacterMovement()->MaxWalkSpeed = PD.StatusData.MoveSpeed;
	Owner->SetDefaultSpeed(PD.StatusData.MoveSpeed);
	Owner->UpdateStatus();
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

void UAPCrowdControlComponent::KnockBackState(FVector KnockBackPoint, float KnockBackDist, float KnockBackTime)
{
	if(!CalculateStateTime(ECharacterState::KnockBack, KnockBackTimerHandle, KnockBackTime)){return;}

	GetWorld()->GetTimerManager().SetTimer(KnockBackTimerHandle, this, &UAPCrowdControlComponent::KnockBackEnd, KnockBackTime, false);

	// KnockBackDist = KnockBackDist; 
	OnCharacterState(ECharacterState::KnockBack, true);

	auto Owner = Cast<ACharacter>(GetOwner()); if(!Owner) return;
	FVector KnockBackVec = (KnockBackPoint - Owner->GetActorLocation()) * FVector(1.0f, 1.0f, 0.0f);
	KnockBackVec = -( KnockBackVec / KnockBackVec.Size());
	DefaultSlip = Owner->GetCharacterMovement()->BrakingFrictionFactor;

	Owner->GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Owner->LaunchCharacter(KnockBackVec * KnockBackDist, false, false);
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

	OnCharacterState(ECharacterState::Stun, true);

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

void UAPCrowdControlComponent::FrozenState(float FrozenTime)
{
	if(!CalculateStateTime(ECharacterState::Frozen, FrozenTimerHandle, FrozenTime)){return;}
	GetWorld()->GetTimerManager().SetTimer(FrozenTimerHandle, this, &UAPCrowdControlComponent::FrozenEnd, FrozenTime, false);

	OnCharacterState(ECharacterState::Frozen, true);

	PlayStateEffect(ECharacterState::Frozen, true);
}

void UAPCrowdControlComponent::FrozenEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(FrozenTimerHandle);

	ClearStopState();
	StateMap.Remove(ECharacterState::Frozen);
	CC_Priority.Last((uint8)ECharacterState::Frozen) = false;
	PlayStateEffect(ECharacterState::Frozen, false);

	NormalState();
}

void UAPCrowdControlComponent::SleepState()
{
	// CurrentState = 3;
	// bCanMove = false;
	// GetWorldTimerManager().SetTimer(SleepTimerHandle, this, &UAPCrowdControlComponent::NormalState, State_Time, false);
}

float UAPCrowdControlComponent::GetDefaultSpeed()
{
	float Speed = 0.0f;
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner());
	if(OwnerCharacter) Speed = OwnerCharacter->GetDefaultSpeed_Origin();

	return Speed;
}

void UAPCrowdControlComponent::SlowState(int32 SlowPercent, float SlowTime)
{
	SlowPercent = FMath::Min(SlowPercent, 99);
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner()); if(!OwnerCharacter) return;
	float Speed = GetDefaultSpeed();

	OnCharacterState(ECharacterState::Slow, false);

	PlayStateEffect(ECharacterState::Slow, true);

	if(CurrentSlowCoefficient > (1.0f - SlowPercent*0.01f))
	{
		CurrentSlowCoefficient = (1.0f - SlowPercent*0.01f);
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed * CurrentSlowCoefficient * CurrentFastCoefficient;
	}
	SlowPriority.Add(SlowPercent);

	OwnerCharacter->SetCanJog(false);
	OwnerCharacter->SetDefaultSpeed(OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed);

	FTimerHandle SlowTimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAPCrowdControlComponent::SlowEnd, &SlowTimerHandle, SlowPercent);
	GetWorld()->GetTimerManager().SetTimer(SlowTimerHandle, TimerDelegate, SlowTime, false);
}

void UAPCrowdControlComponent::SlowEnd(FTimerHandle* SlowTimerHandle, int32 SlowPercent)
{
	float Speed = GetDefaultSpeed();

	SlowPriority.RemoveSingle(SlowPercent);
	if(SlowPriority.Num() > 1) SlowPriority.Sort([](const uint8& A, const uint8& B) {return A < B;});
	
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner()); if(!OwnerCharacter) return;

	if(SlowPriority.IsEmpty())
	{
		CurrentSlowCoefficient = 1.0f;
		OwnerCharacter->SetCanJog(true);

		StateMap.Remove(ECharacterState::Slow);
		CC_Priority.Last((uint8)ECharacterState::Slow) = false;
		PlayStateEffect(ECharacterState::Slow, false);

		NormalState();
	}
	else
	{
		CurrentSlowCoefficient = (1.0f - SlowPriority.Top()*0.01f);
	}

	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed * CurrentSlowCoefficient * CurrentFastCoefficient;
	OwnerCharacter->SetDefaultSpeed(OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed);

	GetWorld()->GetTimerManager().ClearTimer(*SlowTimerHandle);
}

void UAPCrowdControlComponent::FastState(float FastCoefficient, bool Start)
{
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner()); if(!OwnerCharacter) return;
	float Speed = GetDefaultSpeed();

	if(Start) {CurrentFastCoefficient = CurrentFastCoefficient + FastCoefficient;}
	else {CurrentFastCoefficient = CurrentFastCoefficient - FastCoefficient;}

	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed * CurrentSlowCoefficient * CurrentFastCoefficient;
	OwnerCharacter->SetDefaultSpeed(OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed);
}

void UAPCrowdControlComponent::FastState(float FastCoefficient, float FastTime)
{
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner()); if(!OwnerCharacter) return;
	float Speed = GetDefaultSpeed();

	CurrentFastCoefficient = CurrentFastCoefficient + FastCoefficient;
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed * CurrentSlowCoefficient * CurrentFastCoefficient;
	OwnerCharacter->SetDefaultSpeed(OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed);

	FTimerHandle FastTimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAPCrowdControlComponent::FastEnd, &FastTimerHandle, FastCoefficient);
	GetWorld()->GetTimerManager().SetTimer(FastTimerHandle, TimerDelegate, FastTime, false);
}

void UAPCrowdControlComponent::FastEnd(FTimerHandle* FastTimerHandle, float FastCoefficient)
{
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner()); if(!OwnerCharacter) return;
	
	DefaultSpeed = GetDefaultSpeed();
	CurrentFastCoefficient = CurrentFastCoefficient - FastCoefficient; CurrentFastCoefficient = FMath::Max(1.0f, CurrentFastCoefficient);
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed * CurrentSlowCoefficient * CurrentFastCoefficient;
	OwnerCharacter->SetDefaultSpeed(OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed);

	GetWorld()->GetTimerManager().ClearTimer(*FastTimerHandle);
}

void UAPCrowdControlComponent::BurnState(APawn* DamageCauser, float DOT, float TotalTime, float InRate)
{
	BurnEnd();
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAPCrowdControlComponent::OnBurnDamage, DamageCauser, DOT);
	GetWorld()->GetTimerManager().SetTimer(BurnTimerHandle, TimerDelegate, InRate, true);
	GetWorld()->GetTimerManager().SetTimer(BurnEndTimerHandle, this, &UAPCrowdControlComponent::BurnEnd, TotalTime, false);
}

void UAPCrowdControlComponent::OnBurnDamage(APawn* DamageCauser, float DOT)
{
	auto MyOwnerInstigator = DamageCauser->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	UGameplayStatics::ApplyDamage(GetOwner(), DOT, MyOwnerInstigator, DamageCauser, DamageTypeClass);
	// Burn Effect 추가
}

void UAPCrowdControlComponent::BurnEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(BurnTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(BurnEndTimerHandle);
}

void UAPCrowdControlComponent::BleedingState(APawn* DamageCauser, float BleedingDamage, float TotalTime, float InRate)
{
	BleedingEnd();
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAPCrowdControlComponent::OnBleedingDamage, DamageCauser, BleedingDamage);
	GetWorld()->GetTimerManager().SetTimer(BleedingTimerHandle, TimerDelegate, InRate, true);
	GetWorld()->GetTimerManager().SetTimer(BleedingEndTimerHandle, this, &UAPCrowdControlComponent::BleedingEnd, TotalTime, false);
}

void UAPCrowdControlComponent::OnBleedingDamage(APawn* DamageCauser, float BleedingDamage)
{
	auto MyOwnerInstigator = DamageCauser->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	UGameplayStatics::ApplyDamage(GetOwner(), BleedingDamage, MyOwnerInstigator, DamageCauser, DamageTypeClass);
	// Bleeding Effect 추가
}

void UAPCrowdControlComponent::BleedingEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(BleedingTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(BleedingEndTimerHandle);
}

float UAPCrowdControlComponent::GetDefaultATK()
{
	float ATk = 0.0f;
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner());
	if(OwnerCharacter) ATk = OwnerCharacter->GetDefaultATK();

	return ATk;
}

void UAPCrowdControlComponent::SetDefaultATK(float NewValue)
{
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner());
	if(OwnerCharacter) OwnerCharacter->SetDefaultATK(NewValue);
}

void UAPCrowdControlComponent::WeakState(float WeakCoefficient, float WeakTime)
{
	SetDefaultATK(GetDefaultATK() * (1.0f - WeakCoefficient));
	FTimerHandle WeakTimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAPCrowdControlComponent::WeakEnd, &WeakTimerHandle, WeakCoefficient);
	GetWorld()->GetTimerManager().SetTimer(WeakTimerHandle, TimerDelegate, WeakTime, false);
}

void UAPCrowdControlComponent::WeakEnd(FTimerHandle* WeakTimerHandle, float WeakCoefficient)
{
	SetDefaultATK(GetDefaultATK() / (1.0f - WeakCoefficient));
	GetWorld()->GetTimerManager().ClearTimer(*WeakTimerHandle);
}

void UAPCrowdControlComponent::OnCharacterState(ECharacterState UpdateState, bool IsStop)
{
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner());
	if(!OwnerCharacter) return;

	// 우선순위 체크
	if ((uint8)OwnerCharacter->returnState() > (uint8)UpdateState || (uint8)OwnerCharacter->returnState() == 0)
	{
		OwnerCharacter->SetState(UpdateState);
		CC_Priority.Last((uint8)UpdateState) = true;
		if(IsStop) OwnerCharacter->StopState.Add(true);
		if(OwnerCharacter->ActorHasTag(TEXT("Enemy"))) OwnerCharacter->OnCrowdControlCheck.Broadcast();
	}

}

void UAPCrowdControlComponent::ClearStopState()
{
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner());
	if(!OwnerCharacter) return;
	if(!OwnerCharacter->StopState.IsEmpty()) OwnerCharacter->StopState.Pop();

	if(OwnerCharacter->ActorHasTag(TEXT("Enemy"))) OwnerCharacter->OnCrowdControlCheck.Broadcast();
}

void UAPCrowdControlComponent::SwitchingState(ECharacterState UpdateState)
{
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->SetState(UpdateState);
	if(OwnerCharacter->ActorHasTag(TEXT("Enemy"))) OwnerCharacter->OnCrowdControlCheck.Broadcast();
}

void UAPCrowdControlComponent::PlayStateEffect(ECharacterState UpdateState, bool IsPlay)
{
	auto OwnerCharacter = Cast<AAPCharacterBase>(GetOwner());

	switch (UpdateState)
	{
		case ECharacterState::KnockBack:
		break;
		
		case ECharacterState::Stun:
		if(OwnerCharacter) {IsPlay ? OwnerCharacter->GetStunEffect()->Activate() : OwnerCharacter->GetStunEffect()->DeactivateImmediate(); }
		break;

		case ECharacterState::Sleep:
		break;
		
		case ECharacterState::Slow:
		break;

		case ECharacterState::Frozen:
		if(OwnerCharacter) {
			if(IsPlay) {FrozenEffect = GetWorld()->SpawnActor<AAPFreezing>(FrozenEffectClass, OwnerCharacter->GetActorLocation() - FVector(0.0f,0.0f,20.0f), OwnerCharacter->GetActorRotation());}
			else {if(FrozenEffect.IsValid()) FrozenEffect->Destroy();}
		}
		break;
	}
}
