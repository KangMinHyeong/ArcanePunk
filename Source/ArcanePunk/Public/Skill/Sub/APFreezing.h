// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APFreezing.generated.h"

UCLASS()
class ARCANEPUNK_API AAPFreezing : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPFreezing();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FreezingMesh;
	
	UPROPERTY(EditAnywhere)
	FVector TargetScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere)
	float TargetSpeed = 5.0f;
};
