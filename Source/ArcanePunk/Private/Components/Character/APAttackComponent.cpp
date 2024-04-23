
#include "Components/Character/APAttackComponent.h"

#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Character/APHitPointComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "UserInterface/APHUD.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

UAPAttackComponent::UAPAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAPAttackComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAPAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAPAttackComponent::InitAttackComp()
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim.IsValid()) return;
}

void UAPAttackComponent::StartAttack_A(bool & bCanMove)
{
	if(!OwnerCharacter.IsValid()) return;  if(!OwnerAnim.IsValid()) return;
	if(bAttack_A)
	{
		if(!FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo)) return;
		if (CanCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ComboAttackStart();
		OwnerAnim->PlayAttack_A_Montage();
		// OwnerAnim->JumpToComboSection(CurrentCombo);
		bAttack_A = true;
		bCanMove = false;
	}
}

void UAPAttackComponent::StartAttack_B(bool &bCanMove)
{
	if(!OwnerCharacter.IsValid()) return;  if(!OwnerAnim.IsValid()) return;

	if(bAttack_B) return;
	bAttack_B = true;
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), Attack_Sound, GetActorLocation(), E_SoundScale);
	OwnerAnim->PlayAttack_B_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UAPAttackComponent::SetAttack_A(bool NewBool)
{
	bAttack_A = NewBool;
}

void UAPAttackComponent::SetAttack_B(bool NewBool)
{
	bAttack_B = NewBool;
}

bool UAPAttackComponent::GetAttack_A()
{
    return bAttack_A;
}

bool UAPAttackComponent::GetAttack_B()
{
    return bAttack_B;
}

void UAPAttackComponent::ComboAttackStart()
{
	CanCombo = true;
	IsComboInputOn = false;
	if(!FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1)) return;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void UAPAttackComponent::ComboAttackEnd()
{
	CanCombo = false;
	IsComboInputOn = false;
	CurrentCombo = 0;
}

void UAPAttackComponent::ComboCheck()
{
	if(!OwnerCharacter.IsValid()) return;  if(!OwnerAnim.IsValid()) return;

	CanCombo = false;
	if (IsComboInputOn)
	{
		ComboAttackStart();
		OwnerAnim->JumpToComboSection(CurrentCombo);
	}
}

void UAPAttackComponent::SpawnSwordTrail(uint8 ComboStack)
{
	if(!OwnerCharacter.IsValid()) return;	
	if(bSuperiorMode) return;
	
	FRotator PlusRot = FRotator::ZeroRotator;
	switch (ComboStack)
	{
	case 1:
		PlusRot = Combo_1_Rot;
		break;
	
	case 2:
		PlusRot = Combo_2_Rot;
		break;

	case 3:
		PlusRot = Combo_3_Rot;
		break;
	}
	FVector PlusLoc = SwordTrailHeight.X * OwnerCharacter->GetActorForwardVector() + OwnerCharacter->GetActorUpVector()*SwordTrailHeight.Z;
	auto SwordTrail = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SwordTrailEffect, OwnerCharacter->GetActorLocation() + PlusLoc, OwnerCharacter->GetActorRotation() + PlusRot);
	SwordTrail->SetNiagaraVariableFloat(TEXT("AttackSpeed"), OwnerCharacter->GetPlayerStatus().PlayerDynamicData.ATKSpeed);
}

//AttackTrace 코드 시작
bool UAPAttackComponent::AttackTrace(FHitResult &HitResult, FVector &HitVector, FVector Start, bool CloseAttack,  bool Custom, float CustomRadius)
{
	FRotator Rotation = GetOwner()->GetActorRotation();
	FVector End = Start;
	if(CloseAttack) End = End + Rotation.Vector() * MaxDistance + FVector::UpVector* 25.0f; // 캐릭터와 몬스터의 높이차가 심하면 + FVector::UpVector* MonsterHigh
	else End = End + FVector::UpVector* 70.0f;

	// 아군은 타격 판정이 안되게 하는 코드
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Actors);
	for (AActor* Actor : Actors)
    {
		Params.AddIgnoredActor(Actor);
    }    
	
	HitVector = -Rotation.Vector();

	FCollisionShape Sphere;
	if(Custom)
	{
		Sphere= FCollisionShape::MakeSphere(CustomRadius);
	}
	else
	{
		if(CloseAttack) {Sphere = FCollisionShape::MakeSphere(AttackRadius);}
		else {Sphere = FCollisionShape::MakeSphere(AttackRadius*1.25);}
	}

	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params);// 타격 판정 인자 Params 인자 추가
}

bool UAPAttackComponent::MultiAttackTrace(TArray<FHitResult> &HitResult, FVector &HitVector, FVector Start, bool CloseAttack,  bool Custom, float CustomRadius)
{
	FRotator Rotation = GetOwner()->GetActorRotation();
	FVector End = Start;
	if(CloseAttack) End = End + Rotation.Vector() * MaxDistance + FVector::UpVector* 25.0f; // 캐릭터와 몬스터의 높이차가 심하면 + FVector::UpVector* MonsterHigh
	else End = End + FVector::UpVector* 70.0f;
	
	// 아군은 타격 판정이 안되게 하는 코드
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Actors);
	for (AActor* Actor : Actors)
    {
		Params.AddIgnoredActor(Actor);
    }    
	
	HitVector = -Rotation.Vector();

	FCollisionShape Sphere;
	if(Custom)
	{
		Sphere= FCollisionShape::MakeSphere(CustomRadius);
	}
	else
	{
		if(CloseAttack) {Sphere = FCollisionShape::MakeSphere(AttackRadius);}
		else {Sphere = FCollisionShape::MakeSphere(AttackRadius*1.25);}
	}

	return GetWorld()->SweepMultiByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params);// 타격 판정 인자 Params 인자 추가
}

bool UAPAttackComponent::MultiAttackTrace(TArray<FHitResult> &HitResult, FVector &HitVector, FVector Start, FVector End, float Radius, bool ExceptPlayer)
{
	FRotator Rotation = GetOwner()->GetActorRotation();

	// 아군은 타격 판정이 안되게 하는 코드
	FCollisionQueryParams Params;

	if(ExceptPlayer)
	{
		Params.AddIgnoredActor(GetOwner());
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Actors);
		for (AActor* Actor : Actors)
		{
			Params.AddIgnoredActor(Actor);
		}  
	}
	  
	HitVector = -Rotation.Vector();
	
	FCollisionShape Sphere =  FCollisionShape::MakeSphere(Radius);


	// return UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, Radius, TraceTypeQuery3, true, Actors, EDrawDebugTrace::Persistent, HitResult, true);
	// ECollisionChannel
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllDynamicObjects); 
	return GetWorld()->SweepMultiByObjectType(HitResult, Start, End, FQuat::Identity, ObjectQueryParam, Sphere, Params);// 타격 판정 인자 Params 인자 추가
	// 	return GetWorld()->SweepMultiByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params, FCollisionResponseParams(ECR_Block));// 타격 판정 인자 Params 인자 추가
}

void UAPAttackComponent::NormalAttack(FVector Start, bool CloseAttack, float Multiple, bool bStun, float StunTime, bool Custom, float CustomRadius)
{
	if(!OwnerCharacter.IsValid()) return;

	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	float Damage = OwnerCharacter->GetCurrentATK() * Multiple;
	FHitResult HitResult;
	FVector HitVector;
	bool Line = AttackTrace(HitResult, HitVector, Start, CloseAttack, Custom, CustomRadius);
	if(Line)
	{
		if(AActor* Actor = HitResult.GetActor())
		{
			FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
			AController* MyController = Cast<AController>(OwnerCharacter->GetController());
			if(!MyController) return;
			OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);
			if(bStun) OwnerCharacter->GetHitPointComponent()->SetCrowdControl(Actor, ECharacterState::Stun, StunTime);
			float DamageApplied = Actor->TakeDamage(Damage * OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, GetOwner()); DrainCheck(Actor, DamageApplied, DrainCoefficient);
			if(ComboHitEffect && Actor->ActorHasTag(TEXT("Enemy"))) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ComboHitEffect, HitResult.ImpactPoint, FRotator::ZeroRotator, HitEffectScale);
		}
	}
}

void UAPAttackComponent::MultiAttack()
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;
	if(bSuperiorMode) {OnSuperiorAttack.Broadcast(BaseAttackDist, BaseAttackRadius); return;}

	float Damage = OwnerCharacter->GetCurrentATK();
	TArray<FHitResult> HitResults;
	FVector HitVector;
	FVector Start = OwnerCharacter->GetActorLocation()+ OwnerCharacter->GetActorForwardVector() * BaseAttackRadius;
	FVector End = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * BaseAttackDist;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, BaseAttackRadius);
	if(Line)
	{
		TArray<AActor*> Actors;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
				AController* MyController = Cast<AController>(OwnerCharacter->GetController()); if(!MyController) return;
				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);
				float Check = FMath::RandRange(0.0f,100.0f);
				if(Check <= BaseInstantDeathPercent) 
				{
					auto Enemy = Cast<AEnemy_CharacterBase>(Actor);
					if(Enemy) Damage = FMath::Max(Damage, Enemy->GetMonsterHP()); 
				}
				float DamageApplied = Actor->TakeDamage(Damage *  OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, GetOwner()); 
				DrainCheck(Actor, DamageApplied, DrainCoefficient);
			}			
		}
	}
	
}

void UAPAttackComponent::MultiAttack(FVector Start, bool CloseAttack, float Multiple, bool bStun, float StunTime, bool Custom, float CustomRadius)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	float Damage = OwnerCharacter->GetCurrentATK() * Multiple;
	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, CloseAttack, Custom, CustomRadius);
	if(Line)
	{
		TArray<AActor*> Actors;
		for(FHitResult HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}

			if(Actor)
			{
				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
				AController* MyController = Cast<AController>(OwnerCharacter->GetController());
				if(!MyController) return;
				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);
				if(bStun) OwnerCharacter->GetHitPointComponent()->SetCrowdControl(Actor, ECharacterState::Stun, StunTime);
				float DamageApplied = Actor->TakeDamage(Damage * OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, GetOwner()); DrainCheck(Actor, DamageApplied, SkillDrainCoefficient);
			}
		}
	}
}

void UAPAttackComponent::MultiAttack(FVector Start, FVector End, float Radius, float Multiple, uint8 HitNumbers, bool bStun, float StunTime)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	float Damage = OwnerCharacter->GetCurrentATK() * Multiple;
	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius);
	if(Line)
	{
		TArray<AActor*> Actors;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
				AController* MyController = Cast<AController>(OwnerCharacter->GetController()); if(!MyController) return;
				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);
				if(bStun) OwnerCharacter->GetHitPointComponent()->SetCrowdControl(Actor, ECharacterState::Stun, StunTime);
		
				ApplyDamageToActor(Actor, Damage * OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, HitNumbers);
			}			
		}
	}
}

void UAPAttackComponent::MultiAttack(FVector Start, FVector End, float Radius, float Multiple, uint8 HitNumbers, float InstantDeathPercent)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	float Damage = OwnerCharacter->GetCurrentATK() * Multiple;
	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius);
	if(Line)
	{
		TArray<AActor*> Actors;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
				AController* MyController = Cast<AController>(OwnerCharacter->GetController()); if(!MyController) return;
				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);
				float Check = FMath::RandRange(0.0f,100.0f);
				if(Check <= InstantDeathPercent) 
				{
					auto Enemy = Cast<AEnemy_CharacterBase>(Actor);
					if(Enemy) Damage = Enemy->GetMonsterHP(); 
				}
				ApplyDamageToActor(Actor, Damage * OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, HitNumbers);
			}			
		}
	}

    
}

TArray<AActor*> UAPAttackComponent::MultiAttack_Return(FVector Start, FVector End, float Radius, float Multiple, uint8 HitNumbers, float InstantDeathPercent, bool bStun, float StunTime)
{
	TArray<AActor*> Actors;
	if(!OwnerCharacter.IsValid()) return Actors;
	if(OwnerCharacter->returnState() != ECharacterState::None) return Actors;

	float Damage = OwnerCharacter->GetCurrentATK() * Multiple;
	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius);
	if(Line)
	{
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
				AController* MyController = Cast<AController>(OwnerCharacter->GetController()); if(!MyController) return Actors;
				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);
				if(bStun) OwnerCharacter->GetHitPointComponent()->SetCrowdControl(Actor, ECharacterState::Stun, StunTime);
		
				ApplyDamageToActor(Actor, Damage * OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, HitNumbers);
			}			
		}
	}
	return Actors;
}

void UAPAttackComponent::MultiAttack_KnockBack(FVector Start, FVector End, float Radius, float KnockBackDist, float Multiple, uint8 HitNumbers, float InstantDeathPercent, float KnockBackTime, bool PlayerKnockBack)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	float Damage = OwnerCharacter->GetCurrentATK() * Multiple;
	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius, false);
	if(Line)
	{
		TArray<AActor*> Actors;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
				AController* MyController = Cast<AController>(OwnerCharacter->GetController()); if(!MyController) return;
				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);

				float Dist = KnockBackDist - (Actor->GetActorLocation()*FVector(1.0f,1.0f,0.0f) - Start*FVector(1.0f,1.0f,0.0f)).Size();

				OwnerCharacter->GetHitPointComponent()->SetKnockBackVec(Start); 
				OwnerCharacter->GetHitPointComponent()->SetKnockBackDist(Dist); 
				OwnerCharacter->GetHitPointComponent()->SetCrowdControl(Actor, ECharacterState::KnockBack, KnockBackTime);
				
				ApplyDamageToActor(Actor, Damage* OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, HitNumbers);
			}	
			else if(Actor->ActorHasTag(TEXT("Player")))		
			{
				float Dist = KnockBackDist - (OwnerCharacter->GetActorLocation()*FVector(1.0f,1.0f,0.0f) - Start*FVector(1.0f,1.0f,0.0f)).Size();
				if(PlayerKnockBack) OwnerCharacter->GetCrowdControlComponent()->KnockBackState(Start, Dist, 0.35f);
			}
		}
	}

}

TArray<AActor*> UAPAttackComponent::MultiAttack_KnockBack_Return(FVector Start, FVector End, float Radius, float KnockBackDist, float Multiple, uint8 HitNumbers, float InstantDeathPercent, float KnockBackTime, bool PlayerKnockBack)
{
	TArray<AActor*> Actors;
	if(!OwnerCharacter.IsValid()) return Actors;
	if(OwnerCharacter->returnState() != ECharacterState::None) return Actors;

	float Damage = OwnerCharacter->GetCurrentATK() * Multiple;
	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius, false);
	if(Line)
	{
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
				AController* MyController = Cast<AController>(OwnerCharacter->GetController()); if(!MyController) return Actors;
				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);

				float Dist = KnockBackDist - (Actor->GetActorLocation()*FVector(1.0f,1.0f,0.0f) - Start*FVector(1.0f,1.0f,0.0f)).Size();

				OwnerCharacter->GetHitPointComponent()->SetKnockBackVec(Start); 
				OwnerCharacter->GetHitPointComponent()->SetKnockBackDist(Dist); 
				OwnerCharacter->GetHitPointComponent()->SetCrowdControl(Actor, ECharacterState::KnockBack, KnockBackTime);
				
				ApplyDamageToActor(Actor, Damage * OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, HitNumbers);
			}	
			else if(Actor->ActorHasTag(TEXT("Player")))		
			{
				float Dist = KnockBackDist - (OwnerCharacter->GetActorLocation()*FVector(1.0f,1.0f,0.0f) - Start*FVector(1.0f,1.0f,0.0f)).Size();
				if(PlayerKnockBack) OwnerCharacter->GetCrowdControlComponent()->KnockBackState(Start, Dist, 0.35f);
			}
		}
	}
	return Actors;
}

void UAPAttackComponent::MultiAttack_Burn(FVector Start, FVector End, float Radius, float DOT, float TotalTime, float InRate)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius, false);
	if(Line)
	{
		TArray<AActor*> Actors;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				auto Enemy = Cast<AEnemy_CharacterBase>(Actor); 
				if(Enemy) Enemy->GetCrowdControlComp()->BurnState(OwnerCharacter.Get(), DOT, TotalTime, InRate);
			}	
		}
	}
}

void UAPAttackComponent::MultiAttack_Slow(FVector Start, FVector End, float Radius, int32 SlowPercent,float TotalTime)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius, false);
	if(Line)
	{
		TArray<AActor*> Actors;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				auto Enemy = Cast<AEnemy_CharacterBase>(Actor); 
				if(Enemy) Enemy->GetCrowdControlComp()->SlowState(SlowPercent, TotalTime);
			}	
		}
	}
}

void UAPAttackComponent::MultiAttack_OnlyCC(FVector Start, FVector End, float Radius, ECharacterState UpdateState, float TotalTime)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius, false);
	if(Line)
	{
		TArray<AActor*> Actors;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			OwnerCharacter->GetHitPointComponent()->SetCrowdControl(Actor, UpdateState, TotalTime);
		}
	}
}

void UAPAttackComponent::ApplyDamageToActor(AActor* DamagedActor, float Damage, FPointDamageEvent myDamageEvent, AController* MyController, uint8 HitNumbers)
{
	if(HitNumbers <= 0) {return;}
	float DamageApplied = DamagedActor->TakeDamage(Damage, myDamageEvent, MyController, GetOwner()); DrainCheck(DamagedActor, DamageApplied, SkillDrainCoefficient);
	HitNumbers--;
	FTimerHandle Timer;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAPAttackComponent::ApplyDamageToActor, DamagedActor, Damage, myDamageEvent, MyController, HitNumbers);
	GetWorld()->GetTimerManager().SetTimer(Timer, TimerDelegate, 0.2f, false);
}

float UAPAttackComponent::ApplyDamageToActor(AActor* DamagedActor, float Damage, FHitResult HitResult, bool bCriticalApply)
{
	if(!OwnerCharacter.IsValid()) return 0.0f;
	if(bCriticalApply) Damage = Damage * OwnerCharacter->CriticalCalculate();
	FPointDamageEvent myDamageEvent(Damage, HitResult, -GetOwner()->GetActorRotation().Vector(), nullptr); 
	float DamageApplied = DamagedActor->TakeDamage(Damage, myDamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
	return DamageApplied;
}

void UAPAttackComponent::DrainCheck(AActor* DamagedActor, float DamageApplied, float Coeff)
{
	if(!OwnerCharacter.IsValid()) return;
	auto Enemy = Cast<AEnemy_CharacterBase>(DamagedActor); if(!Enemy) return;

	float DrainCoeff = Coeff;

	auto PDD = OwnerCharacter->GetPlayerStatus(); float OriginHP = PDD.PlayerDynamicData.HP;
    
	if(Enemy->IsInDrainField() && OwnerCharacter->GetInArcaneTent())
	{
		DrainCoeff = DrainCoeff + FieldDrainCoefficient;
	}

	float HP = PDD.PlayerDynamicData.HP + DamageApplied * DrainCoeff; 
    PDD.PlayerDynamicData.HP = FMath::Min(PDD.PlayerDynamicData.MaxHP, HP);
	
	OwnerCharacter->SetDefaultHP(PDD.PlayerDynamicData.HP); 
	OwnerCharacter->GetAPHUD()->OnUpdateHPBar.Broadcast(OriginHP);

}

//AttackTrace 코드 끝