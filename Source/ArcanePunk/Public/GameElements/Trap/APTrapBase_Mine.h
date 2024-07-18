// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/Trap/APTrapBase.h"
#include "APTrapBase_Mine.generated.h"

class AArcanePunkCharacter;
class UDecalComponent;

UCLASS()
class ARCANEPUNK_API AAPTrapBase_Mine : public AAPTrapBase
{
	GENERATED_BODY()
public:	
	AAPTrapBase_Mine();

protected:
	virtual void BeginPlay() override;

	void AutoRotating();
	void OperateExplosion(float DeltaTime);
	void Explosion();
public:	
	virtual void Tick(float DeltaTime) override;

  	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MineTopMesh;

	UPROPERTY(EditAnywhere)
	UDecalComponent* MineRangeDecal;

	TWeakObjectPtr<AArcanePunkCharacter> Player;

	UPROPERTY(EditAnywhere)
	float RotateSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float OperateSpeed = 500.0f;

	bool bRotating = true;
	bool bOperation = false;

	FVector OperationLocation;
};
