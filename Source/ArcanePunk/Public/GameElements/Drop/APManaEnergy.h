// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APManaEnergy.generated.h"

class UNiagaraComponent;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ARCANEPUNK_API AAPManaEnergy : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPManaEnergy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
  	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetEnergyMoveComp(AActor* ManaOwner);

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* EnergyTrigger;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* EnergyEffect;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* EnergyMoveComp;

	UPROPERTY(EditAnywhere)
	float HomingSpeed = 750.0f;
};
