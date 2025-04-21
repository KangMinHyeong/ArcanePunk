
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
	if(bDash) TickDash(DeltaTime);
	TickCheck();
}

void UAPMovementComponent::SetBind()
{
	OwnerCharacter = Cast<AAPCharacterBase>(GetOwner());
}

void UAPMovementComponent::PlayerMoveForward(float AxisValue)
{
	PlayerVec.Y = -AxisValue;
	if(PlayerVec.SizeSquared() == 0) return;
	PlayerRot = FRotationMatrix::MakeFromX(PlayerVec).Rotator();

	if(!OwnerCharacter.IsValid() || !OwnerCharacter->GetCanMove() || OwnerCharacter->GetDoing()) return;
	// X축 기준
	OwnerCharacter->GetController()->SetControlRotation(PlayerRot);
	OwnerCharacter->AddMovementInput(PlayerVec);

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
	PlayerVec.X = AxisValue;
	if(PlayerVec.SizeSquared() == 0) return;
	PlayerRot = FRotationMatrix::MakeFromX(PlayerVec).Rotator();
	if(!OwnerCharacter.IsValid() || !OwnerCharacter->GetCanMove() || OwnerCharacter->GetDoing()) return;

	// X축 기준
	OwnerCharacter->GetController()->SetControlRotation(PlayerRot);
	OwnerCharacter->AddMovementInput(PlayerVec);

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
	if(!bMove && !bRotation && !bDash) SetComponentTickEnabled(false);
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
		StopTickMove();
	}

}

void UAPMovementComponent::TickDash(float DeltaTime)
{
	if(!OwnerCharacter.IsValid()) return;
	if(OwnerCharacter->returnState() != ECharacterState::None && OwnerCharacter->returnState() != ECharacterState::Slow) return;

	FVector CurrentLoc = OwnerCharacter->GetActorLocation();
	CurrentLoc = FMath::VInterpConstantTo(CurrentLoc, DashLocation, DeltaTime, DashSpeed);
	OwnerCharacter->SetActorLocation(CurrentLoc, true);

	if((CurrentLoc - DashLocation).Size() < 75.0f) 
	{
		Cast<AArcanePunkCharacter>(OwnerCharacter)->SetAttackCancelTime(0.2f);
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

void UAPMovementComponent::StartTickMove(const FVector & ToLocation)
{
	TargetLocation = OwnerCharacter->GetActorLocation() + ToLocation;
	LocSpeed = PushSpeed;
	bMove = true;
	SetComponentTickEnabled(true);
}

void UAPMovementComponent::StopTickMove()
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

void UAPMovementComponent::SetAttackRotation(const FRotator & NewTargetRot, float AddSpeed)
{
	Current = GetOwner()->GetActorRotation();
	TargetRot = NewTargetRot;
	RotSpeed = InitRotSpeed + AddSpeed;
	// SetComponentTickEnabled(true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAPMovementComponent::RotateMovement, 0.00001f, false);
}

void UAPMovementComponent::StartDash()
{
	OwnerCharacter->SetActorRotation(PlayerRot);
	DashLocation = GetOwner()->GetActorLocation() + PlayerRot.Vector() * DashLength;
	bDash = true;
	bMove = false; 
	SetComponentTickEnabled(true);

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim) return;
	OwnerAnim->PlayDash_Montage();
}

void UAPMovementComponent::SwapDash()
{
	DashLocation = GetOwner()->GetActorLocation() + PlayerRot.Vector() * DashLength;
	bDash = true;
	bMove = false; 
	SetComponentTickEnabled(true);

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim) return;
	OwnerAnim->PlayDash_Montage();
}

void UAPMovementComponent::EndDash()
{
	bDash = false; 

	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()); if(!OwnerAnim) return;
	OwnerAnim->	StopDash_Montage();

	 GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UAPMovementComponent::EndedAttackCancelTime, 0.3f, false);
}

void UAPMovementComponent::EndedAttackCancelTime()
{
	Cast<AArcanePunkCharacter>(OwnerCharacter)->SetAttackCancelTime(0.0f);
}
