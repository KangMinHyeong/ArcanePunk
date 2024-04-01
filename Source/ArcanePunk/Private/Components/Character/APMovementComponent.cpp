
#include "Components/Character/APMovementComponent.h"

#include "GameFramework/Controller.h"
#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Components/Character/APAttackComponent.h"

UAPMovementComponent::UAPMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAPMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	AnimMoveStop();
	InitRotSpeed = RotSpeed;
}

void UAPMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator Current = GetOwner()->GetActorRotation();
	Current = FMath::RInterpConstantTo(Current, TargetRot, DeltaTime, RotSpeed);
	GetOwner()->SetActorRotation(Current);

	if(abs(Current.Yaw - TargetRot.Yaw) < 0.01f) 
	{
		RotSpeed = InitRotSpeed;
		AnimMoveStop();
	}
}

void UAPMovementComponent::PlayerMoveForward(float AxisValue)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter || !OwnerCharacter->GetCanMove()) return;

	PlayerVec.X = AxisValue;
	if(PlayerVec.SizeSquared() != 0)
	{
		OwnerCharacter->GetController()->SetControlRotation(FRotationMatrix::MakeFromX(PlayerVec).Rotator());
		OwnerCharacter->AddMovementInput(PlayerVec);
	}	
}

void UAPMovementComponent::PlayerMoveRight(float AxisValue)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter || !OwnerCharacter->GetCanMove()) return;

	PlayerVec.Y = AxisValue;
	if(PlayerVec.SizeSquared() != 0)
	{
		OwnerCharacter->GetController()->SetControlRotation(FRotationMatrix::MakeFromX(PlayerVec).Rotator());
		OwnerCharacter->AddMovementInput(PlayerVec);
	}
}

void UAPMovementComponent::AttackMoving(float DeltaTime)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if(!OwnerAnim) return;

	int32 Section = OwnerAnim->GetAttackSection();
	float Speed = OwnerCharacter->GetAttackMoveSpeed(Section);

	FHitResult HitResult;
	FVector DashVector = OwnerCharacter->GetActorForwardVector()*Speed * DeltaTime;
	FVector OverlapStart = OwnerCharacter->GetActorLocation();
	FVector OverlapEnd = OverlapStart + OwnerCharacter->GetActorForwardVector() * OwnerCharacter->GetAttackComponent()->GetMaxDistance();

	if(OwnerCharacter->PMCheck(HitResult, OverlapStart, OverlapEnd))
	{
		auto Enemy = Cast<AEnemy_CharacterBase>(HitResult.GetActor());
		if(!Enemy) return;

		if(Enemy->IsHitting())
		{
			if(Enemy->AttackPushBack(DashVector * OwnerCharacter->GetPushCoefficient())) OwnerCharacter->SetActorLocation(OwnerCharacter->GetActorLocation() + DashVector);
		} 
		else
		{
			OwnerCharacter->SetActorLocation(OwnerCharacter->GetActorLocation() + DashVector);
		}
	}
	else
	{
		OwnerCharacter->SetActorLocation(OwnerCharacter->GetActorLocation() + DashVector);
	}
}

void UAPMovementComponent::AnimMovement()
{
	SetComponentTickEnabled(true);
}

void UAPMovementComponent::AnimMoveStop()
{
	SetComponentTickEnabled(false);
}

void UAPMovementComponent::SetAttackRotation(FRotator NewTargetRot, float Speed)
{
	SetComponentTickEnabled(true);
	TargetRot = NewTargetRot;
	if(Speed < 1.0f) return;
	if(abs(Speed - RotSpeed)  >= 1.0f) RotSpeed = Speed;
}
