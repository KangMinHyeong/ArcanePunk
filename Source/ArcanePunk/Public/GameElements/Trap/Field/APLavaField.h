// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APLavaField.generated.h"

class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API AAPLavaField : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPLavaField();

protected:
	virtual void BeginPlay() override;

	void OnFieldDamage();
	
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
	
private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* LavaTrigger;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* LavaEffectComp;

	FTimerHandle DamageTimerHandle;
	UPROPERTY(EditAnywhere)
	float FieldDamage = 1.0f;
	UPROPERTY(EditAnywhere)
	float DamageFrequency = 1.0f;
};
