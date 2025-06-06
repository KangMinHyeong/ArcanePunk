
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
#include "UserInterface/HUD/APHUD.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/Character/APSpringArmComponent.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APCameraComponent.h"

UAPAttackComponent::UAPAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAPAttackComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAPAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	if(OwnerCharacter.IsValid() && OwnerCharacter->bDebugDraw && bParrying)
	{
		UE_LOG(LogTemp, Display, TEXT("Can Parrying"));
	}
}

void UAPAttackComponent::InitAttackComp()
{
	OwnerCharacter = Cast<AAPCharacterBase>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim.IsValid()) return;
}

void UAPAttackComponent::StartComboAttack(float AttackCancelTime)
{
	if(!OwnerCharacter.IsValid()) return;  if(!OwnerAnim.IsValid()) return;
	if(OwnerCharacter->GetDoing()) return;

	if(bComboAttack)
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
		OwnerAnim->PlayCombo_Montage(AttackCancelTime);
		bComboAttack = true;
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
}

void UAPAttackComponent::StartParrying()
{
	if(!OwnerCharacter.IsValid()) return;  if(!OwnerAnim.IsValid()) return;
	if(OwnerCharacter->GetDoing()) return;

	if(bParrying) return;
	bParrying = true;
	Cast<AArcanePunkCharacter>(OwnerCharacter)->SetbCanParrying(false);
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), Attack_Sound, GetActorLocation(), E_SoundScale);
	OwnerAnim->PlayParrying_Montage();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UAPAttackComponent::ComboAttackStart()
{
	CanCombo = true;
	IsComboInputOn = false;
	if(!FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1)) return;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
	
	// if(auto OwnerPlayer = Cast<AArcanePunkCharacter>(OwnerCharacter))
	// {
	// 	OwnerPlayer->OnComboAttackStart.Broadcast(CurrentCombo);
	// }
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
	else
	{
		OwnerAnim->StopComboAttack();
	}
}

bool UAPAttackComponent::CheckParryingCondition(FDamageEvent const &DamageEvent, AController *EventInstigator)
{
	if(!OwnerCharacter.IsValid() || !bParrying) return false;	

	// Parrying
	if(!DamageEvent.IsOfType(FPointDamageEvent::ClassID)) return false;

	FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*) &DamageEvent;
	FVector HitLocation = PointDamageEvent->HitInfo.ImpactPoint;
	HitLocation -= OwnerCharacter->GetActorLocation();
	HitLocation = HitLocation/HitLocation.Size();
	HitLocation.Z = 0.0f;

	FVector Forward = OwnerCharacter->GetActorForwardVector();
	auto DotProduct = FVector::DotProduct(HitLocation, Forward);
	float AngleInDegrees = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

	if(AngleInDegrees > 100.0f) return false;

	if(EventInstigator) ParryCounter(EventInstigator->GetPawn());	
	OnParrying();
	return true;
}

void UAPAttackComponent::OnParrying()
{
	bParrying = false;
	OwnerAnim->PlayParryingSuccess_Montage();
	auto Player = Cast<AArcanePunkCharacter>(OwnerCharacter.Get()); 
	if(Player)
	{
		Player->GetAPSpringArm()->ZoomInterpto(-ParryingZoomDist, ParryingZoomSpeed);
		Player->GetAPCameraComponent()->OnParryingCameraEffect();
		auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController());
		if(PC) PC->ParryingCameraShake();
	}

	FRotator Rot = OwnerCharacter->GetAPMoveComponent()->GetTargetRot(); 
	FVector Loc = OwnerCharacter->GetActorLocation() + Rot.Vector() * OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.0f + OwnerCharacter->GetActorUpVector() * 25.0f;
	Rot.Yaw -= 90.0f;
	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParryingEffect_First, Loc, Rot);

	GetWorld()->GetTimerManager().SetTimer(ParryingTimerHandle, this, &UAPAttackComponent::AffectParrying, AffectParryingTime, false);
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
	SwordTrail->SetVariableFloat(TEXT("AttackSpeed"), OwnerCharacter->GetPlayerStatus().StatusData.ATKSpeed);
}

//AttackTrace 코드 시작
bool UAPAttackComponent::AttackTrace(FHitResult &HitResult, FVector & HitVector, const FVector & Start, bool CloseAttack,  bool Custom, float CustomRadius)
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
	float Rad = CustomRadius;
	if(Custom)
	{
		Sphere= FCollisionShape::MakeSphere(Rad);
	}
	else
	{
		Rad = AttackRadius;
		Sphere = FCollisionShape::MakeSphere(Rad);
	}

	bool bResult = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params);// 타격 판정 인자 Params 인자 추가

	if(OwnerCharacter->bDebugDraw)
	{
		FVector Center = (Start + End) / 2.0f;
		float HalfHeight = MaxDistance * 0.5f + AttackRadius;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(End - Start).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;

		DrawDebugCapsule(GetWorld(),
			Center,
			HalfHeight,
			AttackRadius,
			CapsuleRot,
			DrawColor,
			false,
			3.0f,
			0,
			5);
	}

	return bResult;
}

// bool UAPAttackComponent::MultiAttackTrace(TArray<FHitResult> &HitResult, FVector &HitVector, FVector Start, bool CloseAttack,  bool Custom, float CustomRadius)
// {
// 	FRotator Rotation = GetOwner()->GetActorRotation();
// 	FVector End = Start;
// 	if(CloseAttack) End = End + Rotation.Vector() * MaxDistance + FVector::UpVector* 25.0f; // 캐릭터와 몬스터의 높이차가 심하면 + FVector::UpVector* MonsterHigh
// 	else End = End + FVector::UpVector* 70.0f;
	
// 	// 아군은 타격 판정이 안되게 하는 코드
// 	FCollisionQueryParams Params;
// 	Params.AddIgnoredActor(GetOwner());
// 	TArray<AActor*> Actors;
// 	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Actors);
// 	for (AActor* Actor : Actors)
//     {
// 		Params.AddIgnoredActor(Actor);
//     }    
	
// 	HitVector = -Rotation.Vector();

// 	FCollisionShape Sphere;
// 	float Rad = CustomRadius;
// 	if(Custom)
// 	{
// 		Sphere= FCollisionShape::MakeSphere(Rad);
// 	}
// 	else
// 	{
// 		Rad = AttackRadius;
// 		Sphere = FCollisionShape::MakeSphere(Rad);
// 	}

// 	bool bResult = GetWorld()->SweepMultiByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params);// 타격 판정 인자 Params 인자 추가
	
// 	FVector Center = (Start + End) / 2.0f;
// 	float HalfHeight = MaxDistance * 0.5f + Rad;
// 	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(End - Start).ToQuat();
// 	FColor DrawColor = bResult ? FColor::Green : FColor::Red;

// 	if(OwnerCharacter->bDebugDraw)
// 	{
// 		DrawDebugCapsule(GetWorld(),
// 			Center,
// 			HalfHeight,
// 			Rad,
// 			CapsuleRot,
// 			DrawColor,
// 			false,
// 			3.0f,
// 			0,
// 			5);
// 	}

// 	return bResult;
// }

bool UAPAttackComponent::MultiAttackTrace(TArray<FHitResult> &HitResult, FVector &HitVector, const FVector & Start, const FVector & End, float Radius, bool ExceptPlayer)
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
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllDynamicObjects); 
	
	bool bResult = GetWorld()->SweepMultiByObjectType(HitResult, Start, End, FQuat::Identity, ObjectQueryParam, Sphere, Params);// 타격 판정 인자 Params 인자 추가

	if(OwnerCharacter->bDebugDraw)
	{
		FVector Center = (Start + End) / 2.0f;
		float HalfHeight = MaxDistance * 0.5f + Radius;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(End - Start).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;

		DrawDebugCapsule(GetWorld(),
			Center,
			HalfHeight,
			Radius,
			CapsuleRot,
			DrawColor,
			false,
			3.0f,
			0,
			5);
	}

	return bResult;
}

void UAPAttackComponent::NormalAttack(const FVector & Start, bool CloseAttack, float Multiple, bool bStun, float StunTime, bool Custom, float CustomRadius)
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
			if(Actor->ActorHasTag(TEXT("Enemy"))) AttackCameraShake();
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
		TArray<AActor*> Actors; bool bCheck = false;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			
				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
				AController* MyController = Cast<AController>(OwnerCharacter->GetController()); if(!MyController) return;
				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);
				float Check = FMath::RandRange(0.0f,100.0f);
				if(Check <= BaseInstantDeathPercent) 
				{
					auto Enemy = Cast<AAPCharacterBase>(Actor);
					if(Enemy) Damage = FMath::Max(Damage, Enemy->GetDefaultHP()); 
				}
				float DamageApplied = Actor->TakeDamage(Damage *  OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, GetOwner()); 
				DrainCheck(Actor, DamageApplied, DrainCoefficient);

			if(!bCheck && Actor->ActorHasTag(TEXT("Enemy"))) bCheck = true;	
		}
		if(bCheck) AttackCameraShake();
	}
}

// void UAPAttackComponent::MultiAttack(FVector Start, bool CloseAttack, float Multiple, bool bStun, float StunTime, bool Custom, float CustomRadius)
// {
// 	if(!OwnerCharacter.IsValid()) return;
// 	if(OwnerCharacter->returnState() != ECharacterState::None) return;

// 	float Damage = OwnerCharacter->GetCurrentATK() * Multiple;
// 	TArray<FHitResult> HitResults;
// 	FVector HitVector;
// 	bool Line = MultiAttackTrace(HitResults, HitVector, Start, CloseAttack, Custom, CustomRadius);
// 	if(Line)
// 	{
// 		TArray<AActor*> Actors;
// 		for(FHitResult HitResult : HitResults)
// 		{
// 			AActor* Actor = HitResult.GetActor();
// 			if(Actors.Contains(Actor)) {continue;}
// 			else {Actors.Add(Actor);}

// 			if(Actor)
// 			{
// 				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
// 				AController* MyController = Cast<AController>(OwnerCharacter->GetController());
// 				if(!MyController) return;
// 				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);
// 				if(bStun) OwnerCharacter->GetHitPointComponent()->SetCrowdControl(Actor, ECharacterState::Stun, StunTime);
// 				float DamageApplied = Actor->TakeDamage(Damage * OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, GetOwner()); DrainCheck(Actor, DamageApplied, SkillDrainCoefficient);
// 			}
// 		}
// 	}
// }

void UAPAttackComponent::MultiAttack(const FVector & Start, const FVector & End, float Radius, float Multiple, uint8 HitNumbers, bool bStun, float StunTime)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	float Damage = OwnerCharacter->GetCurrentATK() * Multiple;
	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius);
	if(Line)
	{
		TArray<AActor*> Actors; bool bCheck = false;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				bCheck = true;
				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
				AController* MyController = Cast<AController>(OwnerCharacter->GetController()); if(!MyController) return;
				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);
				if(bStun) OwnerCharacter->GetHitPointComponent()->SetCrowdControl(Actor, ECharacterState::Stun, StunTime);
		
				ApplyDamageToActor(Actor, Damage * OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, HitNumbers);
			}			
		}
		if(bCheck) AttackCameraShake();
	}
}

void UAPAttackComponent::MultiAttack(const FVector & Start, const FVector & End, float Radius, float Multiple, uint8 HitNumbers, float InstantDeathPercent)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	float Damage = OwnerCharacter->GetCurrentATK() * Multiple;
	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius);
	if(Line)
	{
		TArray<AActor*> Actors; bool bCheck = false;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				bCheck = true;
				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
				AController* MyController = Cast<AController>(OwnerCharacter->GetController()); if(!MyController) return;
				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);
				float Check = FMath::RandRange(0.0f,100.0f);
				if(Check <= InstantDeathPercent) 
				{
					auto Enemy = Cast<AAPCharacterBase>(Actor);
					if(Enemy) Damage = Enemy->GetDefaultHP(); 
				}
				ApplyDamageToActor(Actor, Damage * OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, HitNumbers);
			}			
		}
		if(bCheck) AttackCameraShake();
	}  
}

TArray<AActor*> UAPAttackComponent::MultiAttack_Return(const FVector & Start, const FVector & End, float Radius, float Multiple, uint8 HitNumbers, float InstantDeathPercent, bool bStun, float StunTime)
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
		bool bCheck = false;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				bCheck = true;
				FPointDamageEvent myDamageEvent(Damage, HitResult, HitVector, nullptr);
				AController* MyController = Cast<AController>(OwnerCharacter->GetController()); if(!MyController) return Actors;
				OwnerCharacter->GetHitPointComponent()->DistinctHitPoint(HitResult.Location, Actor);
				if(bStun) OwnerCharacter->GetHitPointComponent()->SetCrowdControl(Actor, ECharacterState::Stun, StunTime);
		
				ApplyDamageToActor(Actor, Damage * OwnerCharacter->CriticalCalculate(), myDamageEvent, MyController, HitNumbers);
			}			
		}
		if(bCheck) AttackCameraShake();
	}
	return Actors;
}

void UAPAttackComponent::MultiAttack_KnockBack(const FVector & Start, const FVector & End, float Radius, float KnockBackDist, float Multiple, uint8 HitNumbers, float InstantDeathPercent, float KnockBackTime, bool PlayerKnockBack)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	float Damage = OwnerCharacter->GetCurrentATK() * Multiple;
	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius, false);
	if(Line)
	{
		TArray<AActor*> Actors; bool bCheck = false;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				bCheck = true;
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
				if(PlayerKnockBack) OwnerCharacter->GetCrowdControlComp()->KnockBackState(Start, Dist, 0.35f);
			}
		}
		if(bCheck) AttackCameraShake();
	}

}

TArray<AActor*> UAPAttackComponent::MultiAttack_KnockBack_Return(const FVector & Start, const FVector & End, float Radius, float KnockBackDist, float Multiple, uint8 HitNumbers, float InstantDeathPercent, float KnockBackTime, bool PlayerKnockBack)
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
		bool bCheck = false;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				bCheck = true;
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
				auto OC = Cast<AAPCharacterBase>(Actor); 
				if(OC)
				{
					float Dist = KnockBackDist - (OC->GetActorLocation()*FVector(1.0f,1.0f,0.0f) - Start*FVector(1.0f,1.0f,0.0f)).Size();
					if(PlayerKnockBack) OC->GetCrowdControlComp()->KnockBackState(Start, Dist, 0.35f);
				}
			}
		}
		if(bCheck) AttackCameraShake();
	}
	return Actors;
}

void UAPAttackComponent::MultiAttack_Burn(const FVector & Start, const FVector & End, float Radius, float DOT, float TotalTime, float InRate)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius, false);
	if(Line)
	{
		TArray<AActor*> Actors; bool bCheck = false;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				bCheck = true;
				auto Enemy = Cast<AEnemy_CharacterBase>(Actor); 
				if(Enemy) Enemy->GetCrowdControlComp()->BurnState(OwnerCharacter.Get(), DOT, TotalTime, InRate);
			}	
		}
		if(bCheck) AttackCameraShake();
	}
}

void UAPAttackComponent::MultiAttack_Slow(const FVector & Start, const FVector & End, float Radius, int32 SlowPercent,float TotalTime)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None) return;

	TArray<FHitResult> HitResults;
	FVector HitVector;
	bool Line = MultiAttackTrace(HitResults, HitVector, Start, End, Radius, false);
	if(Line)
	{
		TArray<AActor*> Actors; bool bCheck = false;
		for(FHitResult & HitResult : HitResults)
		{
			AActor* Actor = HitResult.GetActor();
			
			if(Actors.Contains(Actor)) {continue;}
			else {Actors.Add(Actor);}
			if(Actor->ActorHasTag(TEXT("Enemy")))
			{
				bCheck = true;
				auto Enemy = Cast<AEnemy_CharacterBase>(Actor); 
				if(Enemy) Enemy->GetCrowdControlComp()->SlowState(SlowPercent, TotalTime);
			}	
		}
		if(bCheck) AttackCameraShake();
	}
}

void UAPAttackComponent::MultiAttack_OnlyCC(const FVector & Start, const FVector & End, float Radius, ECharacterState UpdateState, float TotalTime)
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

float UAPAttackComponent::ApplyDamageToActor(AActor* DamagedActor, float Damage, FHitResult HitResult, bool bCriticalApply, ECharacterState StateType, float StateTime)
{
	if(!OwnerCharacter.IsValid()) return 0.0f;
	if(bCriticalApply) Damage = Damage * OwnerCharacter->CriticalCalculate();

	if(HitResult.ImpactPoint.X + HitResult.ImpactPoint.Y == KINDA_SMALL_NUMBER) 
	HitResult.ImpactPoint = DamagedActor->GetActorLocation();

	FPointDamageEvent myDamageEvent(Damage, HitResult, GetOwner()->GetActorRotation().Vector(), nullptr); 
	float DamageApplied = DamagedActor->TakeDamage(Damage, myDamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
	
	if(StateType != ECharacterState::None)
	OwnerCharacter->GetHitPointComponent()->SetCrowdControl(DamagedActor, StateType, StateTime);

	OwnerCharacter->UpdateSwapGauge(100.0f);

	return DamageApplied;
}

void UAPAttackComponent::DrainCheck(AActor* DamagedActor, float DamageApplied, float Coeff)
{
	if(!OwnerCharacter.IsValid()) return;
	auto OwnerPlayer = Cast<AArcanePunkCharacter>(OwnerCharacter); if(!OwnerPlayer) return;
	auto Enemy = Cast<AEnemy_CharacterBase>(DamagedActor); if(!Enemy) return;

	float DrainCoeff = Coeff;

	auto PDD = OwnerPlayer->GetPlayerStatus(); float OriginHP = PDD.StatusData.HP;
    
	if(Enemy->IsInDrainField())
	{
		DrainCoeff = DrainCoeff + FieldDrainCoefficient;
	}

	float HP = PDD.StatusData.HP + DamageApplied * DrainCoeff; 
    PDD.StatusData.HP = FMath::Min(PDD.StatusData.MaxHP, HP);
	
	OwnerPlayer->SetDefaultHP(PDD.StatusData.HP); 
	OwnerPlayer->GetAPHUD()->GetStatusWidget()->SetHPPercent(OwnerPlayer, OriginHP);
}

void UAPAttackComponent::ReflectDamage(float & DamageApplied, AActor* DamageCauser)
{
	if(!OwnerCharacter.IsValid()) return;

	FHitResult Hit;
	Hit.Location = OwnerCharacter->GetActorLocation(); Hit.ImpactPoint = Hit.Location;
	UGameplayStatics::ApplyPointDamage(DamageCauser, DamageApplied, Hit.ImpactPoint, Hit, OwnerCharacter->GetInstigatorController(), OwnerCharacter.Get(), UDamageType::StaticClass());
}

void UAPAttackComponent::ParryCounter(AActor * DamageCauser)
{
	auto Enemy = Cast<AEnemy_CharacterBase>(DamageCauser); if(!Enemy) return;
	if(!Enemy->CanParryingCounter()) return;

	FHitResult Hit;
	Hit.Location = OwnerCharacter->GetActorLocation(); Hit.ImpactPoint = Hit.Location;
    FPointDamageEvent myDamageEvent(0.0f, Hit, Hit.ImpactPoint, nullptr);
    DamageCauser->TakeDamage(0.0f, myDamageEvent, OwnerCharacter->GetController(), OwnerCharacter.Get());
}

void UAPAttackComponent::AffectParrying()
{
	GetWorld()->GetTimerManager().ClearTimer(ParryingTimerHandle);
	GetWorld()->GetWorldSettings()->SetTimeDilation(ParryingTimeSlow);

	FRotator Rot = OwnerCharacter->GetAPMoveComponent()->GetTargetRot(); 
	FVector Loc = OwnerCharacter->GetActorLocation() + Rot.Vector() * OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.0f + OwnerCharacter->GetActorUpVector() * 25.0f;
	Rot.Yaw -= 90.0f;
	auto NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParryingEffect, Loc, Rot);

	GetWorld()->GetTimerManager().SetTimer(ParryingTimerHandle, this, &UAPAttackComponent::OnEndingParrying, EndParryingTime, false);
}

void UAPAttackComponent::OnEndingParrying()
{
	GetWorld()->GetTimerManager().ClearTimer(ParryingTimerHandle);
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);

	auto Player = Cast<AArcanePunkCharacter>(OwnerCharacter.Get()); 
	if(Player)
	{
		Player->GetAPSpringArm()->RestoreInterp();
		Player->GetAPCameraComponent()->Restore();
	}
}

void UAPAttackComponent::AttackCameraShake()
{
	auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
	PC->AttackCameraShake();
}

