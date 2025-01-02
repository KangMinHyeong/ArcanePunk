// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APEnemyRescueBoat.generated.h"

class UCapsuleComponent;

UCLASS()
class ARCANEPUNK_API AAPEnemyRescueBoat : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPEnemyRescueBoat();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetRescuePoint(AActor* Actor);

	void OnRescuing();

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* RescueRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RescueMesh;

	UPROPERTY(EditAnywhere)
	float RescueSpeed = 750.0f;

	TWeakObjectPtr<AActor> RescueActor;

	FVector RescuePoint;
	bool bRescue = false;
};
