// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretAmmo.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UAPHitPointComponent;
class UProjectileMovementComponent;

UCLASS()
class ARCANEPUNK_API ATurretAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurretAmmo();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void BindHit();
	void DestroyAmmo();
	UFUNCTION()
	void OnHitting(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit);

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* AmmoMesh;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* AmmoEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* HitEffet;

	FTimerHandle DestroyTimerHandle;

	UPROPERTY(EditAnywhere)
	float DeadTime = 2.0f;

	UPROPERTY()
	UAPHitPointComponent* HitPointComp;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* AmmoMoveComp;

	UPROPERTY(EditAnywhere)
	float AmmoSpeed = 1200.0f;

};
