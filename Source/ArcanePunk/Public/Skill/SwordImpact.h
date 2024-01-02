// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordImpact.generated.h"

UCLASS()
class ARCANEPUNK_API ASwordImpact : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwordImpact();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHitting(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
  	void OnPenetrating(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	void BintHit();
	void DestroyImpact();
	void DamageAction(AActor *OtherActor, const FHitResult &HitResult);
	void SlowPlayer(AActor *OtherActor);
	
private:
	UPROPERTY(EditAnywhere)
	bool IsPenetrate = false;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* ImpactComp;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* BaseEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* HitEffect;

	FTimerHandle DestroyTimerHandle;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float SlowTime = 3.5f;

	UPROPERTY(EditAnywhere)
	float DamageCoefficient = 1.0f;

	UPROPERTY(EditAnywhere)
	float SlowCoefficient = 0.75f;

	UPROPERTY(EditAnywhere)
	float ImpactSpeed = 1500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	class UProjectileMovementComponent* ImpactMovementComponent;
	
	UPROPERTY()
	class UAPHitPointComponent* HitPointComp;

	UPROPERTY(EditAnywhere)
	bool IsPlayerSkill = true;
};
