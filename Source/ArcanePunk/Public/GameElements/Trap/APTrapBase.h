// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APTrapBase.generated.h"

class USphereComponent;
class UNiagaraSystem;
class AArcanePunkCharacter;
class UDecalComponent;

UCLASS()
class ARCANEPUNK_API AAPTrapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPTrapBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
  	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void AutoRotating();

protected:
	// Component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TrapMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* TrapTrigger;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RotateMesh;

	UPROPERTY(EditAnywhere)
	UDecalComponent* RangeDecal;

	// Trap Operate Effect
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* TrapOverlapEffect;

	// Damage
	UPROPERTY(EditAnywhere)
	float TrapDamage = 25.0f;

	// Rotation
	UPROPERTY(EditAnywhere)
	float RotateSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float RotatePlus = 0.0f;
	
	bool bRotating = true;

	// Trap Overlapped Player
	TWeakObjectPtr<AArcanePunkCharacter> Player;
};
