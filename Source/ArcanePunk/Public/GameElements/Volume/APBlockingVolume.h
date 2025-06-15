// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APBlockingVolume.generated.h"

class UBoxComponent;
class UNavModifierComponent;

UCLASS()
class ARCANEPUNK_API AAPBlockingVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAPBlockingVolume();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BlockingVolum;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UNavModifierComponent* NavModifier;

};
