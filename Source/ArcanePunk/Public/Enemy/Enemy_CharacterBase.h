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

	// 몬스터 Dead 관련 함수
	UFUNCTION(BlueprintPure)
	bool IsDead();

	UFUNCTION(BlueprintPure)
	bool IsNormalAttack();

	virtual void PossessedBy(AController* NewController) override;
	void NormalAttack();

	float GetDistanceLimit();

	void TeleportMarkActivate(float Time, AActor * MarkOwner);
	void TeleportMarkDeactivate();

	bool IsHitting();

	bool AttackPushBack(FVector NewLocation);

private:
	float DamageMath(float Damage);
	bool AttackTrace(FHitResult &HitResult, FVector &HitVector);
	void ResetNormalAttack();
	void ResetHitStiffness();
	void SpawnDamageText(float Damage);

	//드롭 관련 함수
	void DropItemActor();

	// 몬스터 Dead 관련 함수
	void EnemyDestroyed();

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
	FTimerHandle HitTimerHandle;
	FTimerHandle DeathTimerHandle;

	UPROPERTY(EditAnywhere)
	float NormalAttack_CastingTime = 1.2f;

	UPROPERTY()
	class UAP_EnemyBaseAnimInstance* Anim;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* TeleportMark;

	UPROPERTY()
	bool bHitting = false;

	UPROPERTY(EditAnywhere)
	float HitStiffnessTime = 0.75f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool IsAttackPush = true;

	// 머터리얼
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Skin")
	UMaterialInterface* HitMaterial; // 피격 점멸 머터리얼

	float DefaultSlip;

	// 드롭 관련 변수
	UPROPERTY(EditAnywhere, Category = "Drop")
	TSubclassOf<class AEnemy_DropBase> DropActorClass;

	UPROPERTY(EditAnywhere, Category = "Drop")
	float DropPercent = 100.0f;

	bool OnDrop = false;

	UPROPERTY(EditAnywhere)
	FRotator DropRot = FRotator(0,0,90);

	UPROPERTY(EditAnywhere, Category = "Drop")
	float DropAngleMax = 45.0f;

	// 몬스터 Dead 관련 변수
	UPROPERTY(EditAnywhere, Category = "Dead")
	float DeathLoadingTime = 3.0f;

	bool bIsDead = false;

	AActor* MarkActor;

protected:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Weapon;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DamageTextClass;
};
