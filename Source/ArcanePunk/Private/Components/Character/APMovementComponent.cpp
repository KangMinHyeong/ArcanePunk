
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
	SetComponentTickEnabled(false);
	InitRotSpeed = RotSpeed;
}

void UAPMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(bMove) TickMove(DeltaTime);
	if(bRotation) TickRotate(DeltaTime);
	TickCheck();
}

void UAPMovementComponent::SetBind()
{
	OwnerCharacter = Cast<AAPCharacterBase>(GetOwner());
}

void UAPMovementComponent::PlayerMoveForward(float AxisValue)
{
	if(!OwnerCharacter.IsValid() || !OwnerCharacter->GetCanMove()) return;

	// X축 기준
	PlayerVec.Y = -AxisValue;
	if(PlayerVec.SizeSquared() != 0)
	{
		OwnerCharacter->GetController()->SetControlRotation(FRotationMatrix::MakeFromX(PlayerVec).Rotator());
		OwnerCharacter->AddMovementInput(PlayerVec);
	}

	// Y축 기준
	// PlayerVec.X = AxisValue;
	// if(PlayerVec.SizeSquared() != 0)
	// {
	// 	OwnerCharacter->GetController()->SetControlRotation(FRotationMatrix::MakeFromX(PlayerVec).Rotator());
	// 	OwnerCharacter->AddMovementInput(PlayerVec);
	// }	
}

void UAPMovementComponent::PlayerMoveRight(float AxisValue)
{
	if(!OwnerCharacter.IsValid() || !OwnerCharacter->GetCanMove()) return;

	// X축 기준
	PlayerVec.X = AxisValue;
	if(PlayerVec.SizeSquared() != 0)
	{
		OwnerCharacter->GetController()->SetControlRotation(FRotationMatrix::MakeFromX(PlayerVec).Rotator());
		OwnerCharacter->AddMovementInput(PlayerVec);
	}

	// Y축 기준
	// PlayerVec.Y = AxisValue;
	// if(PlayerVec.SizeSquared() != 0)
	// {
	// 	OwnerCharacter->GetController()->SetControlRotation(FRotationMatrix::MakeFromX(PlayerVec).Rotator());
	// 	OwnerCharacter->AddMovementInput(PlayerVec);
	// }
}

void UAPMovementComponent::TickCheck()
{
	if(!bMove && !bRotation) SetComponentTickEnabled(false);
}

void UAPMovementComponent::TickRotate(float DeltaTime)
{
	if(!OwnerCharacter.IsValid()) return;
	Current = FMath::RInterpConstantTo(Current, TargetRot, DeltaTime, RotSpeed  * OwnerCharacter->GetPlayerStatus().StatusData.ATKSpeed );
	GetOwner()->SetActorRotation(Current);
	
	if(abs(Current.Yaw - TargetRot.Yaw) < 0.01f) 
	{
		RotSpeed = InitRotSpeed;
		RotateMoveStop();
	}
}

void UAPMovementComponent::TickMove(float DeltaTime)
{
	if(!OwnerCharacter.IsValid()) return;
	FVector Location = GetOwner()->GetActorLocation();
	Location = FMath::VInterpConstantTo(Location, TargetLocation, DeltaTime, LocSpeed * OwnerCharacter->GetPlayerStatus().StatusData.ATKSpeed );
	GetOwner()->SetActorLocation(Location, true);
	if(abs(Location.X - TargetLocation.X) < 0.01f && abs(Location.Y - TargetLocation.Y) < 0.01f) 
	{
		ComboMoveStop();
	}
}

void UAPMovementComponent::ComboMovement()
{
	if(!OwnerCharacter.IsValid()) return;
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim) return;
	int32 Section = OwnerAnim->GetAttackSection();
	switch (Section)
	{
		case 2:
		TargetLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * Combo_2_Distance;
		LocSpeed = Combo_2_Speed;
		break;
	
		case 3:
		TargetLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * Combo_3_Distance;
		LocSpeed = Combo_3_Speed;
		break;
	}
	
	bMove = true;
	SetComponentTickEnabled(true);
}

void UAPMovementComponent::ComboMoveStop()
{
	bMove = false;
}

void UAPMovementComponent::RotateMovement()
{
	bRotation = true;
	SetComponentTickEnabled(true);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UAPMovementComponent::RotateMoveStop()
{
	bRotation = false;
}

void UAPMovementComponent::SetAttackRotation(FRotator NewTargetRot, float Speed)
{
	Current = GetOwner()->GetActorRotation();
	TargetRot = NewTargetRot;
	if(Speed < 1.0f) {RotSpeed = InitRotSpeed;}
	else if(abs(Speed - RotSpeed)  >= 1.0f) RotSpeed = Speed;
	// SetComponentTickEnabled(true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAPMovementComponent::RotateMovement, 0.00001f, false);
}

