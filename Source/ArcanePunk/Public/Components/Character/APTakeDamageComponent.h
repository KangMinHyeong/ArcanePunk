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

	void DamageCalculation(float & DamageApplied);

private:
	void OnHitting();

private:
	FTimerHandle HitTimerHandle;
};
