// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy_CharacterBase.generated.h"

#define Defense_constant 1000

class ATextRenderActor;

UCLASS()
class ARCANEPUNK_API AEnemy_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy_CharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<ATextRenderActor*> PresentDamages;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintPure)
	bool IsDead();

	UFUNCTION(BlueprintPure)
	bool IsNormalAttack();

	virtual void PossessedBy(AController* NewController) override;
	void NormalAttack();

	float GetDistanceLimit();

	void RemovePresentDamage();

	void TeleportMarkActivate(float Time);
	void TeleportMarkDeactivate();

private:
	float DamageMath(float Damage);
	bool AttackTrace(FHitResult &HitResult, FVector &HitVector);
	void ResetNormalAttack();
	void SpawnDamageText(float Damage);

private:
	UPROPERTY(EditAnywhere)
	float HP = 100.0f;

	UPROPERTY(EditAnywhere)
	float Character_Defense = 0.0f;

	UPROPERTY(EditAnywhere)
	float Monster_AttackRange = 250.0f;

	UPROPERTY(EditAnywhere)
	float Monster_AttackRadius = 80.0f;

	UPROPERTY(EditAnywhere)
	float Monster_ATK = 25.0f;

	UPROPERTY(EditAnywhere)
	float Distance_Limit = 800.0f;

	bool bNormalAttack = false;

	FTimerHandle NormalAttackTimerHandle;

	FTimerHandle PresentDamageTimerHandle;

	FTimerHandle TeleportTimerHandle;

	UPROPERTY(EditAnywhere)
	float NormalAttack_CastingTime = 1.2f;

	UPROPERTY()
	class UAP_EnemyBaseAnimInstance* Anim;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* TeleportMark;

protected:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Weapon;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DamageTextClass;
};
