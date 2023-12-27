// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APTakeDamageComponent.generated.h"

#define Defense_constant 1000

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPTakeDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPTakeDamageComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DamageCalculation(float &DamageApplied);

	void SetHitPoint(float Forward, float Right);

	UFUNCTION(BlueprintPure)
	float GetForward();

	UFUNCTION(BlueprintPure)
	float GetRight();

private:
	void OnHitting();
	void TestHit(); // 맞는 방향 확인용 후에 삭제

private:
	FTimerHandle HittingTimerHandle;

	float PlayerIsForward = 0.0f;
	float PlayerIsRight = 0.0f;
};
