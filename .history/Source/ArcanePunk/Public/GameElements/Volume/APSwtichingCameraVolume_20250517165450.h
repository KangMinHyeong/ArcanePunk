// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APSwtichingCameraVolume.generated.h"

class UBoxComponent;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API AAPSwtichingCameraVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAPSwtichingCameraVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	void SwitchingCamera(AArcanePunkCharacter* Character, bool bBackView);

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* AreaTrigger;


};
