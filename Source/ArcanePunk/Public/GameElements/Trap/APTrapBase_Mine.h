// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/Trap/APTrapBase.h"
#include "APTrapBase_Mine.generated.h"


UCLASS()
class ARCANEPUNK_API AAPTrapBase_Mine : public AAPTrapBase
{
	GENERATED_BODY()
public:	
	AAPTrapBase_Mine();

protected:
	virtual void BeginPlay() override;

	virtual void OnTrapOperation_MontageEnded() override;
public:	
	virtual void Tick(float DeltaTime) override;

  	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnDamageTrigger() override;

private:

};
