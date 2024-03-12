// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portal/Portal_Base.h"
#include "LevelPoratl.generated.h"

class AArcanePunkPlayerController;

UCLASS()
class ARCANEPUNK_API ALevelPoratl : public APortal_Base
{
	GENERATED_BODY()
public:	
	ALevelPoratl();

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual FInteractData GetInteractData() override;
	virtual void Interact(AArcanePunkCharacter* PlayerCharacter) override;

protected:
	virtual void BeginPlay() override;
	virtual void SpawnSound(FVector Location) override;
	void StartLoading();

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
	
	TWeakObjectPtr<AArcanePunkPlayerController> CharacterPC;

	UPROPERTY(EditAnywhere)
	FName UnlockedStageName = "";
};
