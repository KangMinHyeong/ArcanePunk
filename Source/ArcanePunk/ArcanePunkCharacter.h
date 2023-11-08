// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArcanePunkCharacter.generated.h"

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

	UPROPERTY(EditAnywhere)
	float Attack_CastingTime = 0.5f;

	UPROPERTY(EditAnywhere)
	float Skill_CastingTime = 1.0f;

	UPROPERTY(EditAnywhere)
	float JoggingSpeed = 700.0f;

	float DefaultSpeed = 400.0f;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* Skill_Q_Effect;

};
