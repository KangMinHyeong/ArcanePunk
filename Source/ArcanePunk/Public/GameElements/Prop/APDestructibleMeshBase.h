// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APDestructibleMeshBase.generated.h"

class UGeometryCollectionComponent;
class UFieldSystemComponent;
class USphereComponent;
class URadialFalloff;
class UCullingField;
class URadialVector;
class UAPManaDropComponent;

UCLASS()
class ARCANEPUNK_API AAPDestructibleMeshBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPDestructibleMeshBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	void Explode();

protected:
	UPROPERTY(EditAnywhere)
	UGeometryCollectionComponent* APDestructibleMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* APDestructionSphere;

	UPROPERTY(EditAnywhere)
	UFieldSystemComponent* APFieldSystem;

	UPROPERTY(EditAnywhere)
	UCullingField* APCullingField;

	UPROPERTY(EditAnywhere)
	URadialFalloff* APRadialFalloff;

	UPROPERTY(EditAnywhere)
	URadialVector* APRadialVector;

	UPROPERTY(EditAnywhere)
	UAPManaDropComponent* APManaDropComp;

	UPROPERTY(EditAnywhere)
	float HP = 1.0f;

	UPROPERTY(EditAnywhere)
	float FalloffCoefficient = 100.0f;
	UPROPERTY(EditAnywhere)
	float VectorCoefficient = 100.0f;
};
