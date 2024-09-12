// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APTrapBase.generated.h"

class USphereComponent;
class UNiagaraSystem;
class AArcanePunkCharacter;
class AAPEnemyAttackRange;
class USkeletalMeshComponent;
class UAPDestructibleMeshComponent;
class UAPTransparentComponent;

UCLASS()
class ARCANEPUNK_API AAPTrapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPTrapBase();
	FORCEINLINE bool IsActivate() const {return bActivate;};
	
protected:
	virtual void BeginPlay() override;

	virtual void OnTrapOperation_MontageEnded();
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
  	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void TrapMontageEnded(UAnimMontage *Montage, bool bInterrupted);
	UFUNCTION(BlueprintCallable)
	bool IsOperationEnd() const {return bOperationEnd;};

	void AutoRotating();
	void AutoDestroy();
	virtual void OnDamageTrigger();
	virtual void Deactivate();

protected:
	// Component
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* RootMesh;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* TopMesh;

	UPROPERTY(EditAnywhere)
	UAPDestructibleMeshComponent* DestructibleMesh;

	UPROPERTY(EditAnywhere)
	UAPTransparentComponent* FadeComp;

	UPROPERTY(EditAnywhere)
	USphereComponent* TrapCollision;

	// Trap Operate Effect
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* TrapOverlapEffect;

	// Damage
	UPROPERTY(EditAnywhere)
	float TrapDamage = 25.0f;

	// Rotation
	UPROPERTY(EditAnywhere)
	float RotateSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float RotatePlus = 0.0f;
	
	bool bRotating = true;
	bool bCharging = false;

	// Trap Range
	UPROPERTY(EditAnywhere)
	TSubclassOf<AAPEnemyAttackRange> RangeClass;
	
	// Trap Overlapped Player
	TWeakObjectPtr<AArcanePunkCharacter> Player;

	FTimerHandle DestroyTimerHandle;
	UPROPERTY(EditAnywhere)
	float DestroyTime = 0.0f;

	bool bOperationEnd = false;
	float OperationTime = 0.0f;

	bool bActivate = true;
};
