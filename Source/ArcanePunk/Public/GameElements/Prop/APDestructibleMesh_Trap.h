// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/Prop/APDestructibleMeshBase.h"
#include "APDestructibleMesh_Trap.generated.h"

class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AAPDestructibleMesh_Trap : public AAPDestructibleMeshBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
  	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void AutoDestroy();

private:
	// Trap Operate Effect
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* TrapOverlapEffect;

	FTimerHandle DestroyTimerHandle;
	UPROPERTY(EditAnywhere)
	float DestroyTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float TrapDamage = 25.0f;
};
