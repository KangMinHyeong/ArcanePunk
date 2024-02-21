// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portal/Portal_Base.h"
#include "LevelPoratl.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API ALevelPoratl : public APortal_Base
{
	GENERATED_BODY()
public:	
	ALevelPoratl();

protected:
	virtual void BeginPlay() override;

	virtual void OnTeleport_A(UPrimitiveComponent*OverlappedComp, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	void Loading();

	// Test
	void Test_StageUnlocking();

private:
	UPROPERTY(EditAnywhere)
	FName NextLevel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> FadeLoadingWidgetClass;

	FTimerHandle LoadTimerHandle;
	
	class AArcanePunkPlayerController* CharacterPC;

	UPROPERTY(EditAnywhere)
	FName UnlockedStageName = "";
};
