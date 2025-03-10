// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APManaDropComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPManaDropComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPManaDropComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnManaDrop(AActor* ManaOwner);

	FORCEINLINE void SetDropPercent(float Percent) {DropPercent = Percent;};
	FORCEINLINE void SetManaAmount(uint8 Amount) {ManaAmount = Amount;};

private:
	UPROPERTY(EditAnywhere)
	float DropPercent = 0.0f;  // 0.0f ~ 100.0f

	UPROPERTY(EditAnywhere)
	uint8 ManaAmount = 1;
		
};
