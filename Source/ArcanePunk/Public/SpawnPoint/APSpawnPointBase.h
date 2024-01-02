// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APSpawnPointBase.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API AAPSpawnPointBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPSpawnPointBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UCapsuleComponent* GetBlockingArea() {return BlockingArea;}; // BlockingArea 반환

private:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* BlockingArea;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* SpawnPointEffect;
};
