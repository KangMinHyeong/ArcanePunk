// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "APManaEnergy.generated.h"

class UNiagaraComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AAPManaEnergy : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPManaEnergy();

protected:
	virtual void BeginPlay() override;

	void SetStartImpulse();
	void StartTracePlayer(float DeltaTime);

	void PlayEnergySound(bool Start);

	FORCEINLINE void StartHoming() {bHoming = true;};
	
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
  	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetEnergyMoveComp(AActor* ManaOwner);

	UFUNCTION()
	void TimeLineUpdateFunc(FVector & Output);
	UFUNCTION()
	void SpawnTimeLineUpdateFunc(FVector & Output);

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* EnergyTrigger;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* EnergyEffect;

	UPROPERTY(EditAnywhere)
	float HomingSpeed = 750.0f;
	float CurrentHomingSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float SpeedUp = 60.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* EnergyOverlapEffect;

	UPROPERTY(EditAnywhere)
	UCurveVector* TimelineCurve;
	UPROPERTY(EditAnywhere)
	UCurveVector* SpawnTimeLineCurve;

	FTimeline TimeLine;
	FTimeline SpawnTimeLine;

	FOnTimelineVector TimeLineUpdateDelegate;
	FOnTimelineVector SpawnTimeLineUpdateDelegate;

	UPROPERTY(EditAnywhere)
	float MaxLength = 2.0f;

	TWeakObjectPtr<AActor> TraceActor;

	bool bHoming = false;

	FTimerHandle TimerHandle;

	FVector SpawnImpulse;
	
	// Sound
	// Sound
	UPROPERTY(EditAnywhere)
	USoundBase* ManaSound_Start;
	UPROPERTY(EditAnywhere)
	USoundBase* ManaSound_Overlap;
	UPROPERTY(EditAnywhere)
	float ManaSoundVolume = 1.0f;
};
