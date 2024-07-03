// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "APEnemyDronCharacter.generated.h"

UCLASS()
class ARCANEPUNK_API AAPEnemyDronCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAPEnemyDronCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Dron")
		int32 MaxDronCount = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Dron")
		int32 DronCount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Dron")
		float DronSpawnRadius;

	UFUNCTION()
		void SpawnDron();

	FTimerHandle SpawnTimerHandle;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
