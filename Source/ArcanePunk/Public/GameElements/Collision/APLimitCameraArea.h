// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APLimitCameraArea.generated.h"

class UBoxComponent;
class AArcanePunkCharacter;
class UAPFadeOutTriggerComponent;

UCLASS()
class ARCANEPUNK_API AAPLimitCameraArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPLimitCameraArea();

protected:
	virtual void BeginPlay() override;

	bool CheckLimit(float X, float Y);

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
	

	FORCEINLINE void ResetInit() {X_Init = 0.0f; Y_Init = 0.0f;};

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* AreaTrigger;

	float X_Init = 0.0f;
	float Y_Init = 0.0f;
	
	float AreaAngle = 0.0f;
	float AreaDegree = 0.0f;

	TWeakObjectPtr<UAPFadeOutTriggerComponent> FadeOutTrigger;
	float InitCameraWidth = 0.0f;

	float CameraHeight = 0.0f;
	float CameraWidth = 0.0f;

	FRotator SpringArmRot = FRotator::ZeroRotator;
	FRotator FadeTriggerRot = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere)
	float InitSpeed = 200.0f;

	FVector FixedSpringArmLocation;

	TWeakObjectPtr<AArcanePunkCharacter> Player;
};
