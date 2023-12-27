// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordThrowBase.generated.h"

UCLASS()
class ARCANEPUNK_API ASwordThrowBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwordThrowBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnHitting(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void DestroySword();
	void ScaleSet();
	
protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Sword;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* TrailEffect;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* HitTrigger;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* HitEffect;

	FTimerHandle DestroyTimerHandle;
	FTimerHandle ScaleTimerHandle;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float ScaleTime = 0.001f;

	UPROPERTY(EditAnywhere)
	float DamageCoefficient = 1.0f;

	UPROPERTY(EditAnywhere)
	float SwordSpeed = 1500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	class UProjectileMovementComponent* SwordMovementComponent;

	UPROPERTY()
	class UAPHitPointComponent* HitPointComp;
};
