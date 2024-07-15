// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APTrapBase.generated.h"

class USphereComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AAPTrapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPTrapBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
  	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TrapMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* TrapTrigger;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* TrapOverlapEffect;

};
