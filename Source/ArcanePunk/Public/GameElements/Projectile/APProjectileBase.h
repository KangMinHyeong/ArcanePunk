// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APProjectileBase.generated.h"

class UNiagaraComponent;
class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AAPProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPProjectileBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHitting(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit);

	void SetDestroy(float DestroyTime);
	void SetSpeed(float Speed);
	void SetRadius(float Radius);
	FORCEINLINE void SetDamage(float Damage) {AmmoDamage = Damage;};
private:
	void DestroyExplosion();

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* AmmoRoot;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* AmmoMesh;
	
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* AmmoEffect;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* AmmoMoveComp;

	FTimerHandle DestroyTimerHandle;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DestroyEffect;

	UPROPERTY(EditAnywhere)
	FVector ExplosionScale = FVector(1.0f, 1.0f, 1.0f);

	float AmmoDamage = 0.0f;
};
