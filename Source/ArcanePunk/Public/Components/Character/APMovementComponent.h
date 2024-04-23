// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APMovementComponent.generated.h"

class AArcanePunkCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPMovementComponent();

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
	void ComboMoveStop(); // 콤보어택 Move 끝

	void SetAttackRotation(FRotator NewTargetRot, float Speed = 0.0f);
private:
	void TickCheck();
	void TickRotate(float DeltaTime); // 틱 회전 함수
	void TickMove(float DeltaTime); // 틱 이동 함수

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

	TWeakObjectPtr<AArcanePunkCharacter> OwnerCharacter;
};
