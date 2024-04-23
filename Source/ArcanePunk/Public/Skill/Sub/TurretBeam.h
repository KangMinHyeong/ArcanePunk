// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretBeam.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
class UAPHitPointComponent;

UCLASS()
class ARCANEPUNK_API ATurretBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurretBeam();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
  	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DestroyBeam();
public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* BeamTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta=(AllowPrivateAccess="true"))
	UNiagaraComponent* BeamEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UProjectileMovementComponent* BeamMovementComponent;

	UPROPERTY(EditAnywhere)
	UAPHitPointComponent* HitPointComp;

	UPROPERTY(EditAnywhere)
	float BeamSpeed = 1550.0f;

	UPROPERTY(EditAnywhere)
	float EnhanceCoefficient = 1.5f;

	UPROPERTY(EditAnywhere)
	float DeadTime = 3.5f;

	FTimerHandle DestroyTimerHandle;
};
