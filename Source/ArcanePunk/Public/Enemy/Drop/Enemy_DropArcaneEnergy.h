// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "Enemy_DropArcaneEnergy.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AEnemy_DropArcaneEnergy : public AEnemy_DropBase
{
	GENERATED_BODY()

public:
	AEnemy_DropArcaneEnergy();

	virtual void DropOverlap(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
	int32 MyArcaneEnergy = 10;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* DropMovement;

	UPROPERTY(EditAnywhere)
	float DropSpeed = 500.0f;
};
