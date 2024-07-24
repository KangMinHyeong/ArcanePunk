// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APLimitCameraArea.generated.h"

class UBoxComponent;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API AAPLimitCameraArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPLimitCameraArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
	


private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* AreaTrigger;

	UPROPERTY(EditAnywhere)
	bool X_Limit = false;
	UPROPERTY(EditAnywhere)
	bool Y_Limit = false;

	FVector FixedSpringArmLocation;

	TWeakObjectPtr<AArcanePunkCharacter> Player;
};
