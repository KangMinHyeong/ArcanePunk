// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APMovementComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMovementComp, Log, All)

class AAPCharacterBase;
class APlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPMovementComponent : public UActorComponent
{

	GENERATED_BODY()

public:	
	UAPMovementComponent();
	
	FORCEINLINE void SetTickMove(bool NewBool) {bMove = NewBool;};
	FORCEINLINE float GetDashTime() const {return DashLength / DashSpeed;};
	FORCEINLINE float GetSwapDashTime() const {return (DashLength * 0.5f) / DashSpeed;};
	FORCEINLINE FRotator GetTargetRot() const {return TargetRot;};
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetBind();

	// 기본 이동 함수
	void PlayerMoveForward(float AxisValue);
	void PlayerMoveRight(float AxisValue);
	
	void RotateMovement(); // RotateMovement 시작
	void RotateMoveStop(); // RotateMovement 끝

	void ComboMovement(); // 콤보어택 Move 시작
	
	void StartTickMove(const FVector & ToLocation);
	void StopTickMove(); 

	void SetAttackRotation(const FRotator & NewTargetRot, float AddSpeed = 0.0f);
	
	void StartLookAtEnemy();

	void StartDash();
	void EndDash();

	void PlaySwapDash();
	void EndSwapDash();

private:
	void TickCheck();
	void TickRotate(float DeltaTime); // 틱 회전 함수
	void TickMove(float DeltaTime); // 틱 이동 함수
	void TickDash(float DeltaTime); // 틱 대쉬 함수
	void TickLookAt(float DeltaTime); // 틱 룩앳앳 함수

	void EndedAttackCancelTime();
	
private:
	// 회전 관련 변수
	FRotator Current;
	FRotator TargetRot;

	bool bRotation = false;

	UPROPERTY(EditAnywhere)
	float RotSpeed = 3.0f;

	float InitRotSpeed;

	FTimerHandle TimerHandle;

	// 이동 관련 변수
	FVector PlayerVec = FVector::ZeroVector;
	FVector TargetLocation;
	FRotator PlayerRot;

	bool bMove = false;

	float LocSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float Combo_2_Speed = 3.0f;
	UPROPERTY(EditAnywhere)
	float Combo_2_Distance = 300.0f;

	UPROPERTY(EditAnywhere)
	float Combo_3_Speed = 3.0f;
	UPROPERTY(EditAnywhere)
	float Combo_3_Distance = 300.0f;

	UPROPERTY(EditAnywhere)
	float PushSpeed = 500.0f;

	//@대시 여부
	bool bDash = false;

	//@대시 속도
	UPROPERTY(EditAnywhere)
	float DashSpeed = 500.0f;

	//@대시 거리
	UPROPERTY(EditAnywhere)
	float DashLength = 1000.0f;

	//@대시 목표 위치
	FVector DashLocation;

	bool bLookAt = false;
	FVector TargetLookAt;

	TWeakObjectPtr<AAPCharacterBase> OwnerCharacter;
	TWeakObjectPtr<APlayerController> OwnerPC;

	FTimerHandle DashTimerHandle;


public:
	//@Dash 속도
	FORCEINLINE float GetDashSpeed() const { return DashSpeed; };

	//@Dash 거리
	FORCEINLINE float GetDashLength() const { return DashLength; };

};
