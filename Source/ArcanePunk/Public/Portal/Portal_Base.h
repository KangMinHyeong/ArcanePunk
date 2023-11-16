// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal_Base.generated.h"

UCLASS()
class ARCANEPUNK_API APortal_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	APortal_Base();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void OnTeleport_A(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnTeleport_B(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void StartTeleport();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PortalMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Destination;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* PortalTrigger;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* DestinationTrigger;

	UPROPERTY(EditAnywhere)
	FName PlayerTag;

	FTimerHandle Delay_TimerHandle;

	UPROPERTY(EditAnywhere)
	float Delay_Time = 2.0f;

	class AArcanePunkCharacter* OverlapCharacter;

	bool Start = true;

	UPROPERTY(EditAnywhere)
	bool BothSides = true;
};
