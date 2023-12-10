// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APMovementComponent.generated.h"


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

	// 기본 이동 함수
	void PlayerMoveForward(float AxisValue);
	void PlayerMoveRight(float AxisValue);
	// 콤보 어택 이동 함수
	void AttackMoving(float DeltaTime);

	void AnimMovement(); // 콤보어택 Move 시작
	void AnimMoveStop(); // 콤보어택 Move 끝

private:

private:
	// 이동 관련 변수
	FVector PlayerVec = FVector::ZeroVector;
};
