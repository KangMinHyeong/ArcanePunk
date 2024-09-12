// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/Trap/APTrapBase.h"
#include "APTrapBase_Spear.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AAPTrapBase_Spear : public AAPTrapBase
{
	GENERATED_BODY()
public:	
	AAPTrapBase_Spear();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOperating();
	
	UFUNCTION()
	void OnOperatingEnd();
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void Deactivate() override;
	
private:
	UPROPERTY(EditAnywhere)
	float SpearHeight = 150.0f;

	UPROPERTY(EditAnywhere)
	float TriggerTime = 3.0f;
	UPROPERTY(EditAnywhere)
	float HoldTime = 3.0f;
	UPROPERTY(EditAnywhere)
	float Margin = 10.0f;

	FTimerHandle TriggerTimerHandle;

	FVector InitLocation;
	FVector Destination;

	bool bOperation = false;
	bool bEnd = false;
};
