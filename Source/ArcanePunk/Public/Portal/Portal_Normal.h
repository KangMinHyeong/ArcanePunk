// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portal/Portal_Base.h"
#include "Portal_Normal.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API APortal_Normal : public APortal_Base
{
	GENERATED_BODY()
public:
	APortal_Normal();
protected:	
	virtual void BeginPlay() override;

	virtual void OnTeleport_A(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnTeleport_B(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void StartTeleport() override;

protected:
	UPROPERTY(EditAnywhere)
	bool BothSides = true;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Destination;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* DestinationTrigger;

	bool Start = true;

	UPROPERTY(EditAnywhere)
	FName PlayerTag;

};
