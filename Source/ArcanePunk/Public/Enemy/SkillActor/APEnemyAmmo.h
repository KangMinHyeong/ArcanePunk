// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APEnemyAmmo.generated.h"

class UNiagaraComponent;
class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class AEnemy_CharacterBase;

UCLASS()
class ARCANEPUNK_API AAPEnemyAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPEnemyAmmo();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetOwnerEnemy(float Dist, float Radius);

	UFUNCTION()
	void OnHitting(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit);

private:
	void DestroyExplosion();

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* AmmoRoot;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* AmmoEffect;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* AmmoMoveComp;

	FTimerHandle DestroyTimerHandle;

	UPROPERTY(EditAnywhere)
	float AmmoSpeed = 1200.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DestroyEffect;

	UPROPERTY(EditAnywhere)
	FVector ExplosionScale = FVector(1.0f, 1.0f, 1.0f);

	TWeakObjectPtr<AEnemy_CharacterBase> OwnerEnemy;
};
