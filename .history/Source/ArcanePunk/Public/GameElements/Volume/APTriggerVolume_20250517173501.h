// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APTriggerVolume.generated.h"

class UBoxComponent;

UCLASS()
class ARCANEPUNK_API AAPTriggerVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPTriggerVolume();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerVolume;

};
