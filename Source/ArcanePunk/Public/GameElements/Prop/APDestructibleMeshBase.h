// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APDestructibleMeshBase.generated.h"

class UAPManaDropComponent;
class UAPDestructibleMeshComponent;

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

protected:
	UPROPERTY(EditAnywhere)
	UAPDestructibleMeshComponent* DestructibleMesh;

	UPROPERTY(EditAnywhere)
	UAPManaDropComponent* APManaDropComp;

	UPROPERTY(EditAnywhere)
	float HP = 1.0f;

	FTimerHandle TimerHandle;
	UPROPERTY(EditAnywhere)
	float FadeOutTime = 3.0f;

	UPROPERTY(EditAnywhere)
	bool bCenterExplosion = true;
};
