// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArcanePunkCharacter.generated.h"

enum class ECharacterState : uint8
{
    None        = 0,
    Stun        = 1,
    KnockBack 	= 2,
    Sleep       = 3,
};
//나중에 스킬도 uint8 또는 16으로 만들기

UCLASS()
class ARCANEPUNK_API AArcanePunkCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AArcanePunkCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	bool IsAttack_A();

	UFUNCTION(BlueprintPure)
	bool IsAttack_B();

	UFUNCTION(BlueprintPure)
	bool IsSkill_Q();

	UFUNCTION(BlueprintPure)
	bool IsSkill_E();

	UFUNCTION(BlueprintPure)
	uint8 returnState();

	UFUNCTION()
	void LookCharacter();


private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void ZoomInOut(float AxisValue);
	void Attack_typeA();
	void Attack_typeB();
	void Skill_typeQ();
	void Skill_typeE();
	void StartJog();
	void EndJog();
	void NormalState();
	void StunState();//후에 인자 추가 (상태시간)
	void KnockBackState();//후에 인자 추가 (상태시간)
	void SleepState();//후에 인자 추가 (상태시간)
	void SwitchState(uint8 Current);
	bool AttackTrace(FHitResult &HitResult, FVector &HitVector);
	void NormalAttack(float ATK);
	bool VisionTrace(FHitResult &HitResult);

private:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* MySpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* MyCamera;

	float MaximumSpringArmLength = 0.0f;

	float CurrentArmLength = 0.0f;

	UPROPERTY(EditAnywhere)
	float MinimumSpringArmLength = 150.0f;

	UPROPERTY(EditAnywhere)
	float ZoomCoefficient = 20.0f;

	FVector PlayerVec = FVector::ZeroVector;

	bool bCanMove = true;

	bool bAttack_A = false;
	bool bAttack_B = false;
	bool bSkill_Q = false;
	bool bSkill_E = false;

	FTimerHandle Attack_ATimerHandle;
	FTimerHandle Attack_BTimerHandle;
	FTimerHandle Skill_QTimerHandle;
	FTimerHandle Skill_ETimerHandle;
	FTimerHandle State_ETimerHandle;
	FTimerHandle State_TimerHandle;

	UPROPERTY(EditAnywhere)
	float Attack_CastingTime = 0.5f;

	UPROPERTY(EditAnywhere)
	float Skill_CastingTime = 1.0f;

	UPROPERTY(EditAnywhere)
	float State_Time = 3.0f;

	UPROPERTY(EditAnywhere)

	float JoggingSpeed = 700.0f;

	float DefaultSpeed = 400.0f;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* Skill_Q_Effect;

	uint8 CurrentState = 0;

	float DefaultSlip = 0.0f;

	uint8 CurrentCharacterState = 0;

	float StateTime = 3.0f;

	UPROPERTY(EditAnywhere)
	float MaxDistance = 200.0f;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 40.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AArcanePunkPlayerState> PlayerStateClass;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AArcanePunkPlayerState* MyPlayerState;
};
