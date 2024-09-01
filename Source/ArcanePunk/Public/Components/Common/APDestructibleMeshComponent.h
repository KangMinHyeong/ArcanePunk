// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "APDestructibleMeshComponent.generated.h"

class UFieldSystemComponent;
class USphereComponent;
class URadialFalloff;
class UCullingField;
class URadialVector;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPDestructibleMeshComponent : public UGeometryCollectionComponent
{
	GENERATED_BODY()

public:	
	UAPDestructibleMeshComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void StartFadeOut();
	void Explode(FVector HitLocation);

protected:
	UPROPERTY(EditAnywhere)
	USphereComponent* APDestructionSphere;

	UPROPERTY()
	UFieldSystemComponent* APFieldSystem;
	UPROPERTY()
	UCullingField* APCullingField;
	UPROPERTY()
	URadialFalloff* APRadialFalloff;
	UPROPERTY()
	URadialVector* APRadialVector;

	UPROPERTY(EditAnywhere)
	float FalloffCoefficient = 100.0f;
	UPROPERTY(EditAnywhere)
	float VectorCoefficient = 50.0f;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> Materials;
	float Opacity = 1.0f;

	UPROPERTY(EditAnywhere)
	float FadeOutSpeed = 0.2f;
	float CurrentFadeOutSpeed = 0.0f;
	bool bFadeOut = false;	
};
