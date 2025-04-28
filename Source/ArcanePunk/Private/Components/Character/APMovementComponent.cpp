
#include "Components/Character/APMovementComponent.h"

#include "GameFramework/Controller.h"
#include "Character/ArcanePunkCharacter.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "AnimInstance/ArcanePunkCharacterAnimInstance.h"
#include "Components/Character/APAttackComponent.h"
#include "Kismet/KismetMathLibrary.h"

//YS
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogMovementComp)

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
	if(bLookAt) TickLookAt(DeltaTime);

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
	if(!bMove && !bRotation && !bDash && !bLookAt) SetComponentTickEnabled(false);
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

void UAPMovementComponent::TickLookAt(float DeltaTime)
{
	if(!OwnerCharacter.IsValid()) return;

	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), TargetLookAt);
	FRotator CurrentRot = FMath::RInterpConstantTo(OwnerCharacter->GetActorRotation(), LookAtRot, DeltaTime, RotSpeed);
	GetOwner()->SetActorRotation(CurrentRot);
	
	if(abs(LookAtRot.Yaw - CurrentRot.Yaw) < 1.0f) 
	{
		bLookAt = false;
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
	//@유효성 검사
	if (!OwnerCharacter.IsValid())
	{
		UE_LOGFMT(LogMovementComp, Warning, "대시 틱 처리 중 유효하지 않은 소유자 캐릭터가 감지되었습니다.");
		return;
	}

	//@캐릭터 상태 확인
	if (OwnerCharacter->returnState() != ECharacterState::None && OwnerCharacter->returnState() != ECharacterState::Slow)
	{
		UE_LOGFMT(LogMovementComp, Log, "캐릭터가 대시 가능한 상태가 아닙니다. 현재 상태: {0}", static_cast<int32>(OwnerCharacter->returnState()));
		return;
	}

	//@대시 위치 업데이트
	FVector CurrentLoc = OwnerCharacter->GetActorLocation();
	CurrentLoc = FMath::VInterpConstantTo(CurrentLoc, DashLocation, DeltaTime, DashSpeed);
	OwnerCharacter->SetActorLocation(CurrentLoc, true);

	float DistanceToTarget = (CurrentLoc - DashLocation).Size();

	UE_LOGFMT(LogMovementComp, Verbose, "대시 진행 중: 목표까지 거리: {0}, 현재 위치: {1}, {2}, {3}",
		DistanceToTarget, CurrentLoc.X, CurrentLoc.Y, CurrentLoc.Z);

	//@목적지 근접 체크
	if (DistanceToTarget < 75.0f)
	{
		UE_LOGFMT(LogMovementComp, Log, "대시 목적지에 거의 도달했습니다. 공격 취소 시간을 0.2초로 설정합니다.");
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

/*
*	@breif: 대시/회피 수행
*/
void UAPMovementComponent::StartDash()
{
	//@대시 시작 준비
	UE_LOGFMT(LogMovementComp, Log, "대시 시작을 처리합니다.");

	//@캐릭터 회전 설정
	OwnerCharacter->SetActorRotation(PlayerRot);

	//@대시 목적지 계산
	DashLocation = GetOwner()->GetActorLocation() + PlayerRot.Vector() * DashLength;
	UE_LOGFMT(LogMovementComp, Log, "대시 목적지 계산: {0}, {1}, {2}", DashLocation.X, DashLocation.Y, DashLocation.Z);

	//@대시 상태 설정
	bDash = true;
	bMove = false;
	SetComponentTickEnabled(true);

	//@애니메이션 처리
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if (!OwnerAnim)
	{
		UE_LOGFMT(LogMovementComp, Error, "유효한 애니메이션 인스턴스를 찾을 수 없습니다.");
		return;
	}

	OwnerAnim->PlayDash_Montage();
	UE_LOGFMT(LogMovementComp, Log, "대시 몽타주 재생을 시작합니다.");
}

void UAPMovementComponent::EndDash()
{
	//@대시 종료 처리
	UE_LOGFMT(LogMovementComp, Log, "대시 종료를 처리합니다.");

	//@대시 상태 해제
	bDash = false;

	//@애니메이션 처리
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if (!OwnerAnim)
	{
		UE_LOGFMT(LogMovementComp, Error, "유효한 애니메이션 인스턴스를 찾을 수 없습니다.");
		return;
	}

	OwnerAnim->StopDash_Montage();
	UE_LOGFMT(LogMovementComp, Log, "대시 몽타주 재생을 중지합니다.");


	//@대시 후 공격 취소 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UAPMovementComponent::EndedAttackCancelTime, 0.3f, false);
	UE_LOGFMT(LogMovementComp, Log, "대시 후 공격 취소 타이머를 0.3초로 설정합니다.");

}

void UAPMovementComponent::PlaySwapDash()
{
	//@스왑 대시 시작 준비
	UE_LOGFMT(LogMovementComp, Log, "스왑 대시 시작을 처리합니다.");

	//@대시 목적지 계산
	DashLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorRightVector() * DashLength * 0.5f;
	UE_LOGFMT(LogMovementComp, Log, "스왑 대시 목적지 계산: {0}, {1}, {2}", DashLocation.X, DashLocation.Y, DashLocation.Z);

	//@캐릭터 회전 설정
	OwnerCharacter->SetActorRotation(FRotationMatrix::MakeFromX(GetOwner()->GetActorRightVector()).Rotator());
	UE_LOGFMT(LogMovementComp, Log, "캐릭터 회전을 오른쪽 벡터 방향으로 설정합니다.");

	//@대시 상태 설정
	bDash = true;
	bMove = false;
	SetComponentTickEnabled(true);
	UE_LOGFMT(LogMovementComp, Log, "스왑 대시 상태가 활성화되었습니다.");

	//@애니메이션 처리
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if (!OwnerAnim)
	{
		UE_LOGFMT(LogMovementComp, Error, "유효한 애니메이션 인스턴스를 찾을 수 없습니다.");
		return;
	}

	OwnerAnim->PlayDash_Montage();
	UE_LOGFMT(LogMovementComp, Log, "스왑 대시 몽타주 재생을 시작합니다.");
}

void UAPMovementComponent::EndSwapDash()
{
	//@스왑 대시 종료 처리
	UE_LOGFMT(LogMovementComp, Log, "스왑 대시 종료를 처리합니다.");

	//@대시 상태 해제
	bDash = false;
	UE_LOGFMT(LogMovementComp, Log, "스왑 대시 상태가 비활성화되었습니다.");

	//@애니메이션 처리
	auto OwnerAnim = Cast<UArcanePunkCharacterAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if (!OwnerAnim)
	{
		UE_LOGFMT(LogMovementComp, Error, "유효한 애니메이션 인스턴스를 찾을 수 없습니다.");
		return;
	}

	OwnerAnim->StopDash_Montage();
	UE_LOGFMT(LogMovementComp, Log, "스왑 대시 몽타주 재생을 중지합니다.");
}

void UAPMovementComponent::EndedAttackCancelTime()
{
	GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
	Cast<AArcanePunkCharacter>(OwnerCharacter)->SetAttackCancelTime(0.0f);
}

void UAPMovementComponent::StartLookAt(const FVector &ToLocation)
{
	bLookAt = true;
	TargetLookAt = ToLocation;
}