// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DirectionalLight.h"
#include "Misc/OutputDeviceNull.h"
#include "GameFramework/Actor.h"
#include "APDayAndNightCycle.generated.h"

UCLASS()
class ARCANEPUNK_API AAPDayAndNightCycle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAPDayAndNightCycle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Sky")
		AActor* Sun;
	
	UPROPERTY(EditAnywhere, Category = "Sky")
		ADirectionalLight* LightSource;

	UPROPERTY(EditAnywhere, Category = "Sky")
		float TurnRate;

};
