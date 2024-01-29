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
	FORCEINLINE uint8 GetPortalNumber() const {return PortalNumber;};
	void InitHide(bool IsHidden);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnTeleport_A(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void StartTeleport();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PortalMesh;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* PortalTrigger;

	FTimerHandle Delay_TimerHandle;

	UPROPERTY(EditAnywhere)
	float Delay_Time = 2.0f;

	class AArcanePunkCharacter* OverlapCharacter;

	UPROPERTY(EditAnywhere)
	bool DefaultHidden = false;

	UPROPERTY(EditAnywhere)
	uint8 PortalNumber = 0;
};
