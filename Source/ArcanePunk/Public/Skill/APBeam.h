// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APBeam.generated.h"

UCLASS()
class ARCANEPUNK_API AAPBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAPBeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void StartBeam();

private:
	UPROPERTY(EditAnywhere)
	class UParticleSystem* BeamEffect;

	class UParticleSystemComponent* BeamEffectComp;

	UPROPERTY(EditAnywhere)
	USceneComponent* BeamRoot;

	UPROPERTY(EditAnywhere)
	float BeamDistance = 2500.0f;


};
