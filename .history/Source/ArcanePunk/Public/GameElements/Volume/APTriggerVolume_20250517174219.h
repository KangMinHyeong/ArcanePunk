// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APTriggerVolume.generated.h"

class UBoxComponent;
class UAPGameInstance;

UCLASS()
class ARCANEPUNK_API AAPTriggerVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPTriggerVolume();

protected:
	virtual void BeginPlay() override;

	void Init();

	UFUNCTION()
  	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerVolume;

	TWeakObjectPtr<UAPGameInstance> APGI;

};
