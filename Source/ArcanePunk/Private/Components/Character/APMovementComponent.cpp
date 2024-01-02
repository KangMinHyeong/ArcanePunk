// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/APMovementComponent.h"

#include "GameFramework/Controller.h"
#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"

UAPMovementComponent::UAPMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAPMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	AnimMoveStop();
}

void UAPMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AttackMoving(DeltaTime);
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
	FVector OverlapEnd = OverlapStart + OwnerCharacter->GetActorForwardVector() * OwnerCharacter->GetMaxDistance();

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
